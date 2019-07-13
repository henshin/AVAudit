# AVAudit
A framework for preforming black-box analysis of antivirus emulators

Inspired by [AVLeak by Alexei Bulazel](https://www.usenix.org/conference/woot16/workshop-program/presentation/blackthorne)

## Usage
![example run](https://i.imgur.com/shfbQrC.png)

View avaudit.py for example usage and fingerprints/ for example fingerprint files

current features
- At least 3x as fast as previous implementations
- Uses less CPU resources than previous implementations
- Create universal fingerprints
- Easy to use API
- Template system

## Notes
* Disable cloud/realtime features on any antivirus
* Implemented using mingw-gcc
* I found to get antiviruses to consistently emulate
  * strip from C runtime
  * define custom entrypoint
  * remove any optimisations
