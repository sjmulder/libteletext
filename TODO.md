Plans
-----
Services:
 - [Teletekst Katwijk](http://server.rtvkatwijk.nl:8095/EXEC) (Netherlands)
 - [RTVSLO](https://teletext.rtvslo.si/100/1) (Slovakia)
 - [ARD Teletext im Ersten](https://www.ard-text.de/) (Germany)
 - [ZDFtext](https://teletext.zdf.de/teletext/zdf/) (Germany)

Features
 x Some sort of wire exchange format to facilitate API proxy
     that's the XML format
 - API proxy
 - Interactive mode in CLI

Library wrappers:
 - .NET
 - Swift
 x C++

Platforms:
 - GTK or Qt GUI
 - Win32 CLI port
 - Win32 GUI
 - WinUI app w/ UWP package for Windows 10+ and Xbox
 - iOS/macOS app (requires API proxy due to App Store rules)
 - Web (WASM, requires API proxy due to XHR)

Infra
 - Automated builds of some sort

Bugs
----
General:
 - Support for subpages in the CLI (100-1). When no subpage is given,
   all subpages for that page should be printed, if the output type
   supports it.
 - Curl errors aren't printed (only that that there was a curl error).

NOS:
 - Sixel characters are not mapped.
 - Accented characters are not mapped.
 - Sometimes blank character at start of line instead of background
   color.
