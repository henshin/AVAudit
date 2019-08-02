# AVAudit
A framework for preforming black-box analysis of antivirus emulators

[**Read about the story behind AVAudit**](https://medium.com/@calebcheng_87413/avaudit-fingerprinting-av-emulators-part-1-introduction-7817b333e25c)

Inspired by [AVLeak by Alexei Bulazel](https://www.usenix.org/conference/woot16/workshop-program/presentation/blackthorne)

## Usage

View **avaudit.py** for example usage and fingerprints/ for example fingerprint files

![example run](https://i.imgur.com/shfbQrC.png)

current features
- At least 3x as fast as previous implementations
- Uses less CPU resources than previous implementations
- Create universal fingerprints
- Easy to use API
- Template system

## Antiviruses implemented
* Defender ✅
* Kaspersky 🚧
* ESET 🚧


## Notes
* Disable cloud/realtime features on any antivirus
* Implemented using mingw-gcc
* I found to get antiviruses to consistently emulate
  * strip from C runtime
  * define custom entrypoint
  * remove any optimisations
