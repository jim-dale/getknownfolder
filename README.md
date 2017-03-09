# getknownfolder

Displays the path for the given known folder name.

Usage: `getknownfolder [-?] [-v] [-a|c|d] [Name] [...]`

  -?  Display this help information.
  -v  Display version information.
  -a  [Default] Searches for the name by display name. If the name does not match a display name searches by canonical name.
  -c  Searches for the name by canonical name.
  -d  Searches for the name by display name.

If no parameters are supplied a list of known folders will be displayed in the format:-
`Display Name [Canonical Name]="Path"`