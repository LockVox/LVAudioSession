![License](https://img.shields.io/github/license/ValentinGrim/LockVox)

# LVAudioSession Project
This repository is used to develop an actual VoIP session that will be later integrated into the [LockVox](https://github.com/ValentinGrim/LockVox) project

## Libraries used
1. [ASIO](https://new.steinberg.net/developers/) (Audio Stream Input/Output) - Windows support for low latency hardware communication
2. [PortAudio](http://files.portaudio.com/download.html) - Wrapper for high level hardware communication
3. [Opus](https://opus-codec.org/downloads/) - Audio CODEC module
4. [JRTPLIB](https://github.com/j0r1/JRTPLIB)  - RTP CODEC module

NB : This Qt project has been setup as library project, you will have to develop your own program for testing :)))))

## Workflow (TBF)
![Workflow](https://github.com/hyugzz/LVAudioSession/blob/master/images/LVAS_Client.png)

## Rights and Liscences
As this project is intended to be integrated into LockVox, it therefore has to be fully open source and GPL compliant

## Contact
Feel free to contact me at laveau@et.esiea.fr
