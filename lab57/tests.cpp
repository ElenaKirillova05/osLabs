#define TEST_BUILD

#include <gtest/gtest.h>
#include "send_recv.hpp"
#include "server.cpp"
#include "client.cpp"

TEST(ClientNodeTest, InitTest) {
    ClientNode client_node;
    Init(1, 1070, client_node);
    
    EXPECT_EQ(client_node.id, 1);
    
    EXPECT_TRUE(client_node.context_client_.has_value());
    EXPECT_TRUE(client_node.socket_parent_.has_value());
}

TEST(ClientNodeTest, AppendTest) {
    zmq::context_t context;
    std::optional<zmq::socket_t> socket;
    std::optional<std::uint32_t> node_id;
    std::string message = "create 2";
    
    Append(socket, context, node_id, message);

    EXPECT_TRUE(socket.has_value());
    EXPECT_TRUE(node_id.has_value());
    EXPECT_EQ(node_id.value(), 2);
}

TEST(ServerNodeTest, InitTest) {
    ServerNode server_node;
    Init(server_node);
    
    EXPECT_EQ(server_node.node_id_.size(), 1);
    
    EXPECT_EQ(server_node.ports_.size(), 100);
}

TEST(ServerNodeTest, PingAllTest) {
    ServerNode server_node;
    Init(server_node);
    std::string message = "ping all";
    
    try {
        EXPECT_NO_THROW({
            if (server_node.node_id_.empty()) {
                FAIL() << "Node ID set is empty!";
            }
            AnalizeUser(server_node, message);
        });
    } catch (const std::bad_optional_access& e) {
        FAIL() << "Caught bad_optional_access exception: " << e.what();
    } catch (const std::exception& e) {
        FAIL() << "Caught unexpected exception: " << e.what();
    }
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
