#pragma once
#include "SharedMemory.h"
#include "Client.h"

/**
	Lobby.h
	Purpose: 

	@author Hampus Hallkvist
	@version 0.5 10/04/2019
*/

class SharedMemory;
class Client;

class Lobby {
public:
	Lobby(int id, SharedMemory* shared_memory);
	~Lobby();

	/**
		Method is the main loop for the lobby.
		It calls the loop method every iteration
		and does a cleanup on finish

		@return void
	 */
	void Execute();
	/**
		The lobby loop making sure
		all clients are synced and
		are working together

		@return void
	 */
	void Loop();
	/**
		Initializes the sending state, telling
		client threads to send their data to
		their corresponding socket

		@return void
	 */
	void InitializeSending();
	/**
		Initializes the receiving state, telling
		client threads start receiving data from
		their corresponding socket

		@return void
	 */
	void InitializeReceiving();
	/**
		Cleanup lobby and delete drop all
		connections

		@return void
	 */
	void CleanUp() const;
	/**
		Clears the vector array
		from all previous commands

		@return void
	 */
	void ResetCommandQueue();
	/**
		Broadcasts the call from lobby
		to all clients

		@param lobby Id of the lobby
		@param receiver Id of the targeted client
		@param command Type of command
		@return void
	 */
	void BroadcastCoreCall(int& lobby, int& receiver, int& command) const;
	/**
		Add a client to the lobby

		@param client Connect client object to lobby
		@return void
	*/
	void AddClient(Client* client);
	/**
		Drops and removes a specific
		client from the list
		
		@param id If of client to drop
		@return void
	*/
	void DropClient(int id);
	/**
		Drop all clients awaiting
		drop

		@return void
	 */
	void DropAwaiting();

	// Getters
	State GetState() const { return lobbyState_; };
	std::vector<std::string> GetClientCommands() const { return commandQueue_; };

	// Setters
	void Drop();

private:
	bool running_;

	State lobbyState_ = receiving;
	State internalState_ = none;

	// Maximum lobby connections the server will allow
	int maxConnections_;

	// All clients connected to the lobby
	int connectedClients_;

	// Points to next lobby
	Client* firstClient_ = nullptr;
	Client* lastClient_ = nullptr;

	// Shared pointer to logger
	std::shared_ptr<spdlog::logger> log_;

	// Mutex objects used to for thread safety 

	std::mutex clientStateMtx_;
	std::mutex addCommandMtx_;
	std::mutex coreCallMtx_;

	// Dynamic allocated array holding all clients responses
	std::vector<std::string> commandQueue_;
	
	// Count of clients that have performed a core call
	int coreCallPerformedCount_;

	SharedMemory* sharedMemory_ = nullptr;

	std::vector<int> dropList_;

public:
	// Lobby specific parameters

	// The id of the lobby
	int id;
	// Points to next lobby
	Lobby* next = nullptr;
	// Points to previous lobby
	Lobby* prev = nullptr;
};
