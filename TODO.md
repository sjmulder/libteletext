Plans
-----
Features
 - Some sort of wire exchange format to facilitate API proxy
 - API proxy

Library wrappers:
 - .NET

Platforms:
 - GTK or Qt GUI
 - Win32 CLI port
 - Win32 GUI
 - WinUI app w/ UWP package for Windows 10+ and Xbox
 - iOS/macOS app (requires API proxy due to App Store rules)
 - Web (WASM, requires API proxy due to XHR)

Bugs
----
General:
 - Support for subpages in the CLI (100-1). When no subpage is given,
   all subpages for that page should be printed, if the output type
   supports it.

NOS:
 - Sixel characters are not mapped.
 - Sometimes blank character at start of line instead of background
   color.
