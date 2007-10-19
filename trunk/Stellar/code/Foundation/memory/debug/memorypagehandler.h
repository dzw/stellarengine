#pragma once
#ifndef DEBUG_MEMORYPAGEHANDLER_H
#define DEBUG_MEMORYPAGEHANDLER_H
//------------------------------------------------------------------------------
/**
    @class Debug::MemoryPageHandler
  
    Provide information about memory allocations to debug http server.
    
    (C) 2007 Radon Labs GmbH
*/
#include "http/httprequesthandler.h"

//------------------------------------------------------------------------------
namespace Debug
{
class MemoryPageHandler : public Http::HttpRequestHandler
{
    DeclareClass(MemoryPageHandler);
public:
    /// constructor
    MemoryPageHandler();
    /// return true if the http request is accepted by the request handler
    virtual bool AcceptsRequest(Http::HttpMethod::Code requestMethod, const IO::URI& requestUri);
    /// handle a http request, the handler is expected to fill the content stream with response data
    virtual Http::HttpStatus::Code HandleRequest(Http::HttpMethod::Code requestMethod, const IO::URI& requestUri, const Ptr<IO::Stream>& responseContentStream);        
};

} // namespace Debug
//------------------------------------------------------------------------------
#endif
