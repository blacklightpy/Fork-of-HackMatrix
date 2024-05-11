#include "MultiPlayer/Gui.h"
#include "MultiPlayer/Server.h"
#include "MultiPlayer/Client.h"
#include "imgui.h"
#include <enet/enet.h>

namespace MultiPlayer {

Gui::Gui() : connect(false), address("127.0.0.1"), port(7777) {}

Gui::~Gui() {}

void Gui::Render() {
    const int cAddressSize = 1024;
    static char cAddress[cAddressSize];
    static int selectedMode = 0;
    const char* modes[] = { "Client", "Server" };
    static auto client = Client();
    static auto server = Server();

    ImGui::Combo("Server/Client", &selectedMode, modes, IM_ARRAYSIZE(modes));

    if (selectedMode == 0) {
        ImGui::InputText("Address", cAddress, cAddressSize);
        address = cAddress;
    }

    ImGui::InputInt("Port", &port);

    if (selectedMode == 0) {
      if(client.IsConnected()) {
        ImGui::Text("connected to server");
        if (ImGui::Button("Disconnect")) {
            client.Disconnect();
        }
      } else {
        if (ImGui::Button("Connect as Client")) {
            client.Connect(address, port);
        }
      }
    } else {
      if(server.IsRunning()) {
        server.Poll();
        ImGui::Text("clientCount: %d", (int)server.GetClients().size());
        if (ImGui::Button("Stop Server")) {
          server.Stop();
        }
      } else {
        if (ImGui::Button("Host as Server")) {
            server.Start(port);
        }
      }
    }
}

bool Gui::IsConnectButtonClicked() const {
  return connect;
}

const std::string& Gui::GetAddress() const {
  return address;
}

int Gui::GetPort() const {
  return port;
}

}