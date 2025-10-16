# Pocket Nomad (ESP32‑S3 media hub)

This project is a modification of https://github.com/Jstudner/jcorp-nomad/ and 
adapted to Pocket-Dongle-S3 from aliexpress 
see my other repo (https://github.com/ronenkr/Pocket-Dongle-S3)

An ESP32‑S3 powered, SD‑card based portable media hub with a tiny ST7789 display and LVGL UI. It hosts a Wi‑Fi access point with a captive portal, streams videos/music/books directly from the SD card (HTTP range requests), exposes an OPDS feed for ebooks, includes a simple file manager/uploader, RGB lighting control, and a one‑button USB Mass‑Storage mode to mount the SD card on your computer.

## Hardware

- MCU: ESP32‑S3 (tested with Waveshare ESP32‑S3 boards)
- Display: ST7789 160×80 SPI TFT
- Storage: SD card via SD_MMC (4‑bit wide)
- Boot/User button: GPIO 0 (enter USB‑MSC)

SD_MMC pins (4‑bit):
- CLK 17, CMD 18, D0 16, D1 15, D2 48, D3 47

ST7789 pins (see `Display_ST7789.h`):
- MOSI 11, SCLK 10, DC 13, RST 14, CS not used (−1)

Open case or fan is strongly suggested!
Supplied are 2 STL files for front and back made for a slim 20x20x6 fan

## Features

- Wi‑Fi AP + captive portal (serves SD card hosted site)
- Media streaming with HTTP range requests: videos, music, images, ebooks
- OPDS catalog for `/Books` (EPUB/PDF)
- Auto/single‑shot media library generation (`media.json`)
- Simple file browser + upload/rename/delete/mkdir APIs
- LVGL UI: status, SD usage, SSID, connected user count
- USB Mass‑Storage mode: mount SD over USB; exit returns to media mode

## SD card layout (served by the web server)

At minimum, place your site files on the SD root (not bundled here):

```
/index.html
/appleindex.html          # optional, for Apple captive portal UX
/assets/...               # CSS/JS/images used by your site
/movies.html, music.html, books.html, shows.html, admin.html, ...
```

Optional media folders the firmware scans/serves:

```
/Movies
/Shows/<ShowName>/Episode1.mp4
/Music or /Music/<PlaylistName>/...
/Books   (EPUB, PDF)
/Gallery (images/videos)
/Files   (misc downloads)
```

You can add optional cover images alongside items, e.g. `/Movies/MyMovie.jpg` or `/Books/Title.jpg`.

## Build and flash

Arduino IDE (recommended):
- Install ESP32 by Espressif (ESP32‑S3 support) in Boards Manager
- Select an ESP32‑S3 board (enable PSRAM if available)
- Open `pocket-nomand.ino` and compile/upload

Dependencies (Arduino libraries):
- ESP Async WebServer (and AsyncTCP for ESP32)
- ArduinoJson
- DNSServer (Arduino core)
- LVGL

Note: This repo provides display/LVGL glue; the web UI files are expected on the SD card.

## First boot and Wi‑Fi

- Default AP SSID: `Pocket_Nomad` (can be changed via settings)
- Default password: `password` (change it!)
- Connect a phone/laptop; captive portal will serve `index.html` from the SD card

## USB Mass‑Storage mode

- Press the Boot/User button (GPIO 0) to reboot into USB‑MSC mode
- Or POST to `/enterUsb` to switch programmatically
- In USB‑MSC, the device exposes the SD card to your computer; eject to trigger return to media mode on next boot

## Library generation (media.json)

On boot the firmware can scan the SD and write `media.json` with entries for Movies/Shows/Books/Music/Gallery/Files.

Ways to trigger:
- Persistent: set `autoGenerateMedia` in settings (see API below)
- One‑time: create `/generate_once.flag` (or POST `/generate-media`) → device reboots and generates

Supported extensions:
- Video: mp4, mkv, avi, mov, webm, m4v, wmv, flv, mpg, mpeg, ts, 3gp
- Music: mp3, wav, flac
- Books: pdf, epub, mobi, azw3, txt
- Gallery: common image/video formats

## OPDS catalog

- Root: `GET /opds/root.xml`
- Books: `GET /opds/books.xml` (scans `/Books` for EPUB/PDF)

## HTTP endpoints (selection)

Media and static:
- `GET /media?file=/path/on/SD` — range streaming
- `GET /Books/<file>.epub|.pdf` — direct download
- `GET /playlist.m3u` (alias `/nomad.m3u`) — playlist
- `GET /assets/*`, `/*` — files served from SD (default `index.html`)

File ops:
- `GET /listfiles?dir=/path` — list entries
- `POST /upload` (form field `dir`) — upload
- `POST /rename` — form fields `oldname`, `newname`
- `POST /delete` — form field `filename`
- `POST /mkdir` — form field `dirname`

Settings and system:
- `GET /settings` — current settings JSON
- `POST /settings` — update any of: `rgbMode`, `rgbColor`, `adminPassword`, `wifiSSID`, `wifiPassword`, `brightness`, `autoGenerateMedia`
- `POST /generate-media` — mark one‑time generation and reboot
- `GET /admin-status` — ssid, wifi password, connected users
- `POST /restart` — reboot
- `GET /cpu-temp` — JSON temperature
- `POST /enterUsb` — switch to USB‑MSC mode

Captive portal helpers:
- `GET /hotspot-detect.html` (Apple), `GET /generate_204` (Android)

DLNA discovery placeholders:
- `GET /dlna/device.xml`, `/dlna/description.xml`, `/ssdp/device-desc.xml` (static descriptors)

## LVGL display

- Display: 160×80 ST7789
- Backlight: `Set_Backlight(brightness)`; brightness persisted via settings (`0–255` typical)
- UI shows SSID, SD usage, user count, status flags

## Troubleshooting

- SD init fails: verify SD_MMC pins, 4‑bit wiring, and card format; check `SD_FREQ_KHZ` and try a lower clock
- Captive portal shows nothing: ensure `index.html` and assets are present on SD root
- Streaming stalls: large/fragmented files or weak Wi‑Fi; prefer high‑quality SD cards; range requests require stable SD reads
- USB‑MSC not appearing: use a data‑capable cable; ensure board USB is connected to the S3 USB‑OTG port; press the boot button again to toggle mode

## Notes

- Default settings are stored on SD at `/config/settings.json` and mirrored in RAM at runtime; use the `/settings` API to change
- Some web UI pages are referenced (`movies.html`, `music.html`, etc.) but not included here; provide your own


## Acknowledgements

- Espressif Arduino‑ESP32 core
- LVGL
- ESP Async WebServer + AsyncTCP
- https://github.com/Jstudner/jcorp-nomad
