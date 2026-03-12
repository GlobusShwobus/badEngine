#pragma once

#include "HttpCallback.h"
#include <curl/curl.h>

namespace badHTTP
{
	//user provides a config curl that gets duplicated
	//the duplicate will set it's own CURLOPT_WRITEDATA and CURLOPT_WRITEFUNCTION
	//can be used for POST or GET. whatever the user does really
	HttpResponse perform_custom(CURL* config);
}

