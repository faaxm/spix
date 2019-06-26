/***
 * Copyright (C) Falko Axmann. All rights reserved.
 * Licensed under the MIT license.
 * See LICENSE.txt file in the project root for full license information.
 ****/

#pragma once

#include <Spix/TestServer.h>

namespace spix {

struct AnyRpcServerPimpl;

/**
 * @brief Runs an AnyRPC based XML-RPC server
 *
 * Use this test server to run a XML-RPC server to receive
 * test commands. This allows testing and controlling the
 * application through external scripts (e.g. python with
 * its xml-rpc library).
 */
class AnyRpcServer : public TestServer {
public:
    AnyRpcServer(int anyrpcPort = 9000);
    ~AnyRpcServer() override;

protected:
    void executeTest() override;

private:
    std::unique_ptr<AnyRpcServerPimpl> m_pimpl;
};

} // namespace spix
