/*
 * Scilab ( https://www.scilab.org/ ) - This file is part of Scilab
 * Copyright (C) 2024 - 3DS - Cedric DELAMARRE
 *
 * For more information, see the COPYING file which you should have received
 * along with this program.
 */

#include "webtools_gw.hxx"
#include "context.hxx"

#define MODULE_NAME L"webtools"
extern "C"
{
#include "gw_webtools.h"
}

int WebtoolsModule::Load()
{
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"http_get", &sci_http_get, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"http_post", &sci_http_post, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"http_put", &sci_http_put, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"http_patch", &sci_http_patch, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"http_delete", &sci_http_delete, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"http_upload", &sci_http_upload, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"url_split", &sci_url_split, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"toJSON", &sci_toJSON, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"fromJSON", &sci_fromJSON, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"url_encode", &sci_url_encode, MODULE_NAME));
    symbol::Context::getInstance()->addFunction(types::Function::createFunction(L"url_decode", &sci_url_decode, MODULE_NAME));

    return 1;
}
