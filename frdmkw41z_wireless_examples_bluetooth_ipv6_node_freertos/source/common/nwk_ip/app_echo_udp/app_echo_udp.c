/*! *********************************************************************************
 * \defgroup NWK_IP
 * @{
 ********************************************************************************** */
/*!
* The Clear BSD License
* Copyright 2016-2017 NXP
* All rights reserved.
* 
* \file
*
* \brief 	This is a public source file for the echo protocol application.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted (subject to the limitations in the
* disclaimer below) provided that the following conditions are met:
* 
* * Redistributions of source code must retain the above copyright
*   notice, this list of conditions and the following disclaimer.
* 
* * Redistributions in binary form must reproduce the above copyright
*   notice, this list of conditions and the following disclaimer in the
*   documentation and/or other materials provided with the distribution.
* 
* * Neither the name of the copyright holder nor the names of its
*   contributors may be used to endorse or promote products derived from
*   this software without specific prior written permission.
* 
* NO EXPRESS OR IMPLIED LICENSES TO ANY PARTY'S PATENT RIGHTS ARE
* GRANTED BY THIS LICENSE. THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT
* HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED
* WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
* MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
* DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
* LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
* CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
* SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
* BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
* WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
* OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN
* IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/
 
#include <string.h>

/* FSL Framework */
#include "MemManager.h"
#include "FunctionLib.h"

/* Application */
#include "sockets.h"
#include "session.h"
#include "app_echo_udp.h"

#include "shell.h"

#if UDP_ECHO_PROTOCOL
/*************************************************************************************
**************************************************************************************
* Private macros
**************************************************************************************
*************************************************************************************/
#define ECHO_SERVER_PORT    (7U)
#define ECHO_PAYLOAD        {0x40, 0x41, 0x42, 0x43, 0x44, 0x45, 0x46, 0x47}
#define ECHO_PAYLOAD_SIZE   (8U)

#define ECHO_DELAY          (1000U)

/************************************************************************************
*************************************************************************************
* Private type definitions
*************************************************************************************
************************************************************************************/
typedef enum echoUdpStatus_tag
{
    mEcho_Ok                 = 0x00U,
    mEcho_Timeout            = 0x01U,
    mEcho_Fail               = 0x02U
}echoUdpStatus_t;

/************************************************************************************
*************************************************************************************
* Private functions prototypes
*************************************************************************************
************************************************************************************/
static int8_t ECHO_CreatePacket(void* pData);
static void ECHO_TimerCallback(void *param);
static void ECHO_RetransmitCallback(void *param);

/************************************************************************************
*************************************************************************************
* Private memory declarations
*************************************************************************************
************************************************************************************/

taskMsgQueue_t * pmEchoUdpMsgQueue = NULL;   /*!< Pointer to main thread message queue */

static int32_t mEchoUdpSrvSockFd = -1;// mEchoTcpSrvSockFd;
tmrTimerID_t resumeEchoUdpTimer = gTmrInvalidTimerID_c;
tmrTimerID_t mEchoUdpDelayTimerID = gTmrInvalidTimerID_c;
uint64_t mEchoUdpTimestamp;
static int32_t mSocketFd = -1;
ipAddr_t mEchoUdpSrcIpAddr ={{0}};
ipAddr_t mEchoUdpDstIpAddr ={{0}};
static uint32_t mEchoUdpTimeoutMs = 2000U;
uint16_t mEchoUdpCounter = 0;
static int32_t mEchoUdpPktSize = 32;

/************************************************************************************
*************************************************************************************
* Public functions
*************************************************************************************
************************************************************************************/

void ECHO_ProtocolInit
(
    taskMsgQueue_t * pMsgQueue
)
{
    sockaddrStorage_t portAddr;

    if (pmEchoUdpMsgQueue == NULL)
    {
        pmEchoUdpMsgQueue = pMsgQueue;
        /* Check if the socket has already been created */
        if (mEchoUdpSrvSockFd == -1)
        {
            /* Set local address and local port */
            FLib_MemSet(&portAddr,0,sizeof(sockaddrStorage_t));
            ((sockaddrIn6_t*)&portAddr)->sin6_family = AF_INET6;
            ((sockaddrIn6_t*)&portAddr)->sin6_port = ECHO_SERVER_PORT;
            IP_AddrCopy(&((sockaddrIn6_t*)&portAddr)->sin6_addr, &in6addr_any);

            /* Open UDP socket */
            mEchoUdpSrvSockFd = socket(AF_INET6, SOCK_DGRAM, IPPROTO_UDP);
            if(mEchoUdpSrvSockFd != -1)
            {
                bind(mEchoUdpSrvSockFd,&portAddr,sizeof(sockaddrStorage_t));
                Session_RegisterCb(mEchoUdpSrvSockFd,ECHO_UdpServerService,pMsgQueue);
            }
        }
        if (mSocketFd == -1)
        {
            FLib_MemSet(&portAddr, 0, sizeof(sockaddrStorage_t));
            mSocketFd =  socket(AF_INET6, SOCK_DGRAM, IPPROTO_UDP);
            bind(mSocketFd, &portAddr, sizeof(sockaddrStorage_t));
            Session_RegisterCb(mSocketFd, ECHO_service, NULL);
        }
    }
}


void ECHO_UdpServerService
(
    void *pInData
)
{
    sessionPacket_t *pSessionPacket = (sessionPacket_t*)pInData;

    if(pSessionPacket->packetOpt.security && (pSessionPacket->localAddr.ss_family == AF_INET6))
    {
        ((sockaddrIn6_t*)&pSessionPacket->remAddr)->sin6_flowinfo = BSDS_SET_TX_SEC_FLAGS(1,5);
    }
    (void)sendto(mEchoUdpSrvSockFd,pSessionPacket->pData, pSessionPacket->dataLen, MSG_SEND_WITH_MEMBUFF,
                 &pSessionPacket->remAddr, sizeof(pSessionPacket->remAddr));

    //MEM_BufferFree(pSessionPacket->pData);
    MEM_BufferFree(pSessionPacket);
}

void ECHO_service
(
    void *pInData
)
{
    sessionPacket_t *pSessionPacket = (sessionPacket_t*)pInData;
    uint64_t tempTimestamp;
    static char ipAddr[INET6_ADDRSTRLEN];

    tempTimestamp = TMR_GetTimestamp();
    tempTimestamp -= mEchoUdpTimestamp;
    tempTimestamp /= 1000;
#if SHELL_ENABLED
    ntop(AF_INET6, &(pSessionPacket->remAddr.ss_addr), ipAddr, INET6_ADDRSTRLEN);
    shell_printf("Message received from %s: bytes=%d, time=", ipAddr, pSessionPacket->dataLen);
    shell_writeDec(tempTimestamp);
    shell_write("ms");
    shell_refresh();
#endif

    MEM_BufferFree(pSessionPacket->pData);
    MEM_BufferFree(pSessionPacket);

    TMR_FreeTimer(resumeEchoUdpTimer);
    resumeEchoUdpTimer = gTmrInvalidTimerID_c;

    TMR_FreeTimer(mEchoUdpDelayTimerID);
    mEchoUdpDelayTimerID = gTmrInvalidTimerID_c;
    
    if (mEchoUdpCounter != 0)
    {   
        /* Start timer */
        if(mEchoUdpDelayTimerID == gTmrInvalidTimerID_c)
        {
            mEchoUdpDelayTimerID = TMR_AllocateTimer();
            TMR_StartSingleShotTimer(mEchoUdpDelayTimerID, ECHO_DELAY,
                                    ECHO_RetransmitCallback, NULL);
        }
        
    }    
}


#if SHELL_ENABLED

int8_t ECHO_ShellUdp
(
    uint8_t argc,
    char *argv[]
)
{ 
    sockaddrStorage_t sourceAddr = {{{0}}};;
    int32_t validIpAddr = 0;    
    int8_t result = CMD_RET_SUCCESS;
    char* pValue = NULL;
    mEchoUdpPktSize = 32;
    mEchoUdpTimeoutMs = 2000;
    mEchoUdpCounter = 1;
    char ipAddr[INET6_ADDRSTRLEN];
    
    if (argc == 0)
    {   
        if (gTmrInvalidTimerID_c != resumeEchoUdpTimer)
        {
            TMR_FreeTimer(resumeEchoUdpTimer);
            resumeEchoUdpTimer = gTmrInvalidTimerID_c;
        }

        if (gTmrInvalidTimerID_c != mEchoUdpDelayTimerID)
        {
    
            TMR_FreeTimer(mEchoUdpDelayTimerID);
            mEchoUdpDelayTimerID = gTmrInvalidTimerID_c;
        }
        return result;
    }
    if (gTmrInvalidTimerID_c != resumeEchoUdpTimer)
    {
        /* timer is already allocated */
        result = CMD_RET_FAILURE;
        return result;
    }
    /* Get option value for -s */
    pValue = shell_get_opt(argc, argv, "-s");
    if(pValue)
    {
        mEchoUdpPktSize = NWKU_atoi(pValue);
    }
    /* Get option value for -S */
    pValue = shell_get_opt(argc, argv, "-S");
    if(pValue)
    {
        pton(AF_INET6, pValue, &mEchoUdpSrcIpAddr);
        IP_AddrCopy(&sourceAddr.ss_addr, &mEchoUdpSrcIpAddr);
        bind(mSocketFd, &sourceAddr, sizeof(sockaddrStorage_t));
    }
    /* Get option value for -i */
    pValue = shell_get_opt(argc, argv, "-i");
    if(pValue)
    {
        mEchoUdpTimeoutMs = (uint32_t)NWKU_atoi(pValue);        
    }

    /* Get option value for -c */
    pValue = shell_get_opt(argc, argv, "-c");
    if(pValue)
    {
        mEchoUdpCounter = (uint16_t)NWKU_atoi(pValue);
    }
    
    /* Find "-t" option */
    for (uint32_t i = 0; i < argc; i++)
    {
        if (FLib_MemCmp(argv[i],"-t",2))
        {
             mEchoUdpCounter = 0xFFFF;
             break;
        }
    }

    /* last parameter should be the target IP address */
    if(strchr(argv[argc - 1], ':'))
    {
        validIpAddr = pton(AF_INET6, argv[argc - 1], &mEchoUdpDstIpAddr);
    }
    if(validIpAddr)
    {
        result = ECHO_CreatePacket(NULL);
        if (result == mEcho_Ok)
        {
            shell_printf("Message sent to %s with %u bytes of data:\n\r", 
                ntop(AF_INET6, &mEchoUdpDstIpAddr, ipAddr, sizeof(ipAddr_t)), mEchoUdpPktSize);
            result = CMD_RET_ASYNC;
        }
        else
        {
            shell_write("Transmission Failed!");
            result = CMD_RET_FAILURE;
        }
    }
    
    
    return result;
}
#endif

static void ECHO_HandleTimerCallback
(
    void* pInData
)
{

#if SHELL_ENABLED
    shell_write("Request timed out\r\n");
#endif

    TMR_FreeTimer(resumeEchoUdpTimer);
    resumeEchoUdpTimer = gTmrInvalidTimerID_c;

    TMR_FreeTimer(mEchoUdpDelayTimerID);
    mEchoUdpDelayTimerID = gTmrInvalidTimerID_c;
        
    if (mEchoUdpCounter != 0)
    {
        /* Start timer */
        if(mEchoUdpDelayTimerID == gTmrInvalidTimerID_c)
        {
            mEchoUdpDelayTimerID = TMR_AllocateTimer();
        }
        TMR_StartSingleShotTimer(mEchoUdpDelayTimerID, ECHO_DELAY, ECHO_RetransmitCallback, NULL);
    }
#if SHELL_ENABLED    
    else
    {       
        shell_refresh();
    } 
#endif    
}

static int8_t ECHO_CreatePacket(void* pData)
{   
    sockaddrStorage_t portAddr = {{{0}}};;
    uint8_t *pPacket = NULL;
    uint8_t payload[8] = ECHO_PAYLOAD;    
    int8_t result = mEcho_Ok;
    
    FLib_MemSet(&portAddr,0,sizeof(sockaddrStorage_t));

    pPacket = NWKU_MEM_BufferAlloc(mEchoUdpPktSize);
    if ( NULL != pPacket )
    {
        int32_t offset = 0;
        while(offset <= (mEchoUdpPktSize - 9))
        {
            FLib_MemCpy((void*)(pPacket+offset), payload, ECHO_PAYLOAD_SIZE);
            offset+=ECHO_PAYLOAD_SIZE;
        }
        if(offset != mEchoUdpPktSize -1)
        {
            FLib_MemCpy((void*)(pPacket + offset), payload, mEchoUdpPktSize - offset);
        }
        
        IP_AddrCopy(&((sockaddrIn6_t*)&portAddr)->sin6_addr, &mEchoUdpDstIpAddr);
        ((sockaddrIn6_t*)&portAddr)->sin6_family = AF_INET6;
        ((sockaddrIn6_t*)&portAddr)->sin6_port = ECHO_SERVER_PORT;
        resumeEchoUdpTimer = TMR_AllocateTimer();
        
        if (resumeEchoUdpTimer != gTmrInvalidTimerID_c)
        {                        
            mEchoUdpTimestamp = TMR_GetTimestamp();
            ((sockaddrIn6_t*)&portAddr)->sin6_flowinfo = BSDS_SET_TX_SEC_FLAGS(1, 5);
            if (-1 != sendto(mSocketFd,(void*)pPacket, mEchoUdpPktSize, MSG_SEND_WITH_MEMBUFF, &portAddr, 
                   sizeof(sockaddrStorage_t)))
            {   
                mEchoUdpCounter--;
                TMR_StartSingleShotTimer(resumeEchoUdpTimer, mEchoUdpTimeoutMs, ECHO_TimerCallback,(void*)mSocketFd);                
                result = mEcho_Ok;
            }
            else
            {
                MEM_BufferFree(pPacket);
                result = mEcho_Fail;
            }
        }
        else
        {   
            MEM_BufferFree(pPacket);            
            result = mEcho_Fail;
        }
    }
    else
    {
       result = mEcho_Fail;
    }

    return result;
}


static void ECHO_TimerCallback(void *param)
{
    (void)NWKU_SendMsg(ECHO_HandleTimerCallback, NULL, pmEchoUdpMsgQueue);
}

static void ECHO_RetransmitCallback
(
    void *param
)
{
    (void)NWKU_SendMsg((nwkMsgHandler)ECHO_CreatePacket, NULL, pmEchoUdpMsgQueue);

    TMR_FreeTimer(mEchoUdpDelayTimerID);
    mEchoUdpDelayTimerID = gTmrInvalidTimerID_c;
}

#endif


