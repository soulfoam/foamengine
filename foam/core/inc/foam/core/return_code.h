#pragma once

typedef enum
{
    FE_OK = 0,

    FE_ERR,

    FE_ERR_PATH_NOT_FOUND,
    FE_ERR_DATA_VERSION_MISMATCH,
    FE_ERR_GL_SHADER_COMPILE,
    FE_ERR_GL_SHADER_LINK,

    FE_ERR_NETWORK_BIND,
    FE_ERR_NETWORK_CONNECT,
    FE_ERR_NETWORK_WINSOCK,
    FE_ERR_NETWORK_SEND,
    FE_ERR_NETWORK_READ,
    FE_NETWORK_ALREADY_HANDLED_PACKET,
    FE_NETWORK_CONNECTED,
    FE_NETWORK_DISCONNECTED,
    FE_NETWORK_ACK,
    FE_NETWORK_PING,
    FE_NETWORK_OUT_OF_ORDER,

} FE_RETURN_CODE;

