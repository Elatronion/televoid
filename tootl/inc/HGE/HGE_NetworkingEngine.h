#ifndef HGE_NETWORKING_ENGINE_H
#define HGE_NETWORKING_ENGINE_H

#include <stddef.h>
#include <stdbool.h>

#define HGE_NETWORK_EVENT_CONNECT     0x0
#define HGE_NETWORK_EVENT_DISCONNECT  0x1

#define HGE_MAX_NETWORK_MESSAGES 1000

typedef struct {
  int id;
  bool isLocalPlayer, hasAuthority;
} hge_network_identity;

typedef struct {
  void* enet_peer;
} hge_network_peer;

typedef struct {
  hge_network_peer sender;
  size_t size;
  char* data;
} hge_network_data;

typedef struct {
  int num_messages;
  hge_network_data messages[HGE_MAX_NETWORK_MESSAGES];
} hge_network_events;

hge_network_events* hgeNetworkEventList();

bool hgeNetworkIsHost();
bool hgeNetworkIsConnected();

int hgeNetworkInit();
int hgeNetworkHost(unsigned int port, unsigned int max_connections);
int hgeNetworkConnect(const char* address_str, unsigned int port);
void hgeNetworkPoll();
void hgeNetworkBroadcastPacket(const char* data, size_t size);
void hgeNetworkSendPacket(const char* data, size_t size);
void hgeNetworkSendPacketTo(hge_network_peer network_peer, const char* data, size_t size);
void hgeNetworkDisconnect();
void hgeNetworkCleanUp();

#endif
