# getknownfolder

[![Build Status](https://dev.azure.com/carrewa/getknownfolder/_apis/build/status/jim-dale.getknownfolder?branchName=master)](https://dev.azure.com/carrewa/getknownfolder/_build/latest?definitionId=5&branchName=master)

Displays the path for the given known folder name.

Usage: `getknownfolder [-?] [-v] [-a|c|d] [Name] [...]`
```
  -?  Display this help information.
  -v  Display version information.
  -a  [Default] Searches for the name by display name.
      If the name does not match a display name, searches by canonical name.
  -c  Searches for the name by canonical name.
  -d  Searches for the name by display name.
```

If no parameters are supplied a list of known folders will be displayed in the format:-

`Display Name [Canonical Name]="Path"`
