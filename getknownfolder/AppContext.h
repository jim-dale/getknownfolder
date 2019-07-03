#pragma once

struct AppContext
{
	bool                    showHelp;
	bool                    showVersion;
	bool                    listKnownFolders;
	std::vector<AppCommand> commands;
};
