#include <SFML/Network.hpp>

#include <iostream>

#include <ServerInfo.h>

#include "User.h"

int main()
{
	sf::TcpListener tcpListener{ };
	tcpListener.setBlocking(false);

	if (tcpListener.listen(ServerPort) != sf::Socket::Done)
	{
		const std::string diagnostic = "TcpListener listening error.";

		std::cout << diagnostic << std::endl;

		throw std::runtime_error{ diagnostic };
	}

	std::cout << "Listening at " << ServerIp << " on port " << ServerPort << ".\n";

	std::vector<User> users{ };

	while (true)
	{
		{
			std::unique_ptr<sf::TcpSocket> user = std::make_unique<sf::TcpSocket>();

			if (tcpListener.accept(*user) == sf::Socket::Done)
			{
				users.emplace_back(std::move(user));
			}
		}

		for (auto iterator = users.begin(); iterator != users.end(); )
		{
			User& user = *iterator;

			if (!user.IsConnected())
			{
				iterator = users.erase(iterator);
				continue;
			}

			user.Update();

			++iterator;
		}

		sf::sleep(sf::milliseconds(1));
	}

	return 0;
}