//
// --------------------------------------------------------------------------
//  Gurux Ltd
//
//
//
// Filename:        $HeadURL$
//
// Version:         $Revision$,
//                  $Date$
//                  $Author$
//
// Copyright (c) Gurux Ltd
//
//---------------------------------------------------------------------------
//
//  DESCRIPTION
//
// This file is a part of Gurux Device Framework.
//
// Gurux Device Framework is Open Source software; you can redistribute it
// and/or modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; version 2 of the License.
// Gurux Device Framework is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
// See the GNU General Public License for more details.
//
// More information of Gurux products: http://www.gurux.org
//
// This code is licensed under the GNU General Public License v2.
// Full text may be retrieved at http://www.gnu.org/licenses/gpl-2.0.txt
//---------------------------------------------------------------------------


#ifndef GXCOMMUNICATION_H
#define GXCOMMUNICATION_H

#include <stdio.h>

#if defined(_WIN32) || defined(_WIN64)//Windows includes
#if _MSC_VER > 1400
#define _CRTDBG_MAP_ALLOC
#define CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#endif
#include <tchar.h>
#include <winsock2.h>//Add support for sockets
#include <ws2tcpip.h>
#include <time.h>
#else //Linux includes.
#define INVALID_HANDLE_VALUE -1
#include <unistd.h>
#include <stdlib.h>
#include <errno.h> //Add support for sockets
#include <netdb.h> //Add support for sockets
#include <sys/types.h> //Add support for sockets
#include <sys/socket.h> //Add support for sockets
#include <netinet/in.h> //Add support for sockets
#include <arpa/inet.h> //Add support for sockets
#include <termios.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <fcntl.h>
#endif

#include "../../development/include/GXDLMSSecureClient.h"

class CGXCommunication
{
    GX_TRACE_LEVEL m_Trace;
    CGXDLMSSecureClient* m_Parser;
    int m_socket;
    static const unsigned int RECEIVE_BUFFER_SIZE = 200;
    unsigned char   m_Receivebuff[RECEIVE_BUFFER_SIZE];
    char* m_InvocationCounter;
#if defined(_WIN32) || defined(_WIN64)//Windows includes
    HANDLE			m_hComPort;
    OVERLAPPED		m_osWrite;
    OVERLAPPED		m_osReader;
#else
    int             m_hComPort;
#endif
    uint16_t m_WaitTime;
    int Read(unsigned char eop, CGXByteBuffer& reply);
    /// Read Invocation counter (frame counter) from the meter and update it.
    int UpdateFrameCounter();
public:
    void WriteValue(GX_TRACE_LEVEL trace, std::string line);
    void SetSocket(int socket)
    {
        m_socket = socket;
    }
public:

    CGXCommunication(CGXDLMSSecureClient* pCosem, uint16_t wt, GX_TRACE_LEVEL trace, char* invocationCounter);
    ~CGXCommunication(void);

    //Disconnect from the meter.
    int Disconnect();
    //Release connection to the meter.
    int Release();

    int Close();

    //Get current time as a string.
    static inline void Now(std::string& str)
    {
        time_t tm1 = time(NULL);
        struct tm dt;
        char tmp[10];
        int ret;
#if _MSC_VER > 1000
        localtime_s(&dt, &tm1);
        ret = sprintf_s(tmp, 10, "%.2d:%.2d:%.2d", dt.tm_hour, dt.tm_min, dt.tm_sec);
#else
        dt = *localtime(&tm1);
        ret = sprintf(tmp, "%.2d:%.2d:%.2d", dt.tm_hour, dt.tm_min, dt.tm_sec);
#endif
        str.append(tmp, ret);
    }

    //Send data to the media.
    int SendData(CGXByteBuffer& data);
    //Read data from the media.
    int ReadData(CGXByteBuffer& reply, std::string& str);

    int ReadDLMSPacket(CGXByteBuffer& data, CGXReplyData& reply);
    int ReadDataBlock(CGXByteBuffer& data, CGXReplyData& reply);
    int ReadDataBlock(std::vector<CGXByteBuffer>& data, CGXReplyData& reply);

    int InitializeConnection();
    int GetAssociationView();

    //Read selected object.
    int Read(CGXDLMSObject* pObject, int attributeIndex, std::string& value);

    //Read selected objects.
    int ReadList(
        std::vector<std::pair<CGXDLMSObject*, unsigned char> >& list);

    //Write selected objects.
    int WriteList(
        std::vector<std::pair<CGXDLMSObject*, unsigned char> >& list);

    //Write selected object.
    int Write(
        CGXDLMSObject* pObject,
        int attributeIndex,
        CGXDLMSVariant& value);

    //Write selected object.
    int Write(
        CGXDLMSObject* pObject,
        int attributeIndex);

    //Call action of selected object.
    int Method(
        CGXDLMSObject* pObject,
        int ActionIndex,
        CGXDLMSVariant& value);


    int ReadRowsByRange(
        CGXDLMSProfileGeneric* pObject,
        CGXDateTime& start,
        CGXDateTime& end,
        CGXDLMSVariant& rows);

    int ReadRowsByRange(
        CGXDLMSProfileGeneric* pObject,
        struct tm* start,
        struct tm* end,
        CGXDLMSVariant& rows);

    int ReadRowsByEntry(
        CGXDLMSProfileGeneric* pObject,
        unsigned int Index,
        unsigned int Count,
        CGXDLMSVariant& rows);

    int ReadScalerAndUnits();
    int GetProfileGenericColumns();

    int GetReadOut();
    int GetProfileGenerics();

    /*
    * Read all objects from the meter. This is only example. Usually there is
    * no need to read all data from the meter.
    */
    int ReadAll(char* outputFile);
    };
#endif //GXCOMMUNICATION_H