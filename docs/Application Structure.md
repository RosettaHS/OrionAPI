# Generic Information: Application Structure
## Orion-Native Applications

Unlike most other [Linux-based operating systems,](https://en.wikipedia.org/wiki/Linux_distribution) The Orion Operating System has a custom Native
Application Structure. This is to make the operating system easier to use for beginners, more stable since Applications store all content internally,
and also provides several other utilities to make development for the operating system easier.

## Structure Overview
Orion-Native Applications (OApps) reside in a custom directory structure. All the contents of a given Application, both user-generated and static, are stored within directories inside of the Application.
The top-level directory ends with a `.oapp` extension. If the Application Structure is valid, the Orion File Manager will hide the extension and show the directory as if it were a single, executable file, with a custom icon.

Within the Application's directory resides a list of internal directories used by the Application and OrionAPI.
Not all of the internal directories are required to be populated, however they are required to exist.

The following is the directory structure of an average Orion-Native Application:
```
MyOApp.oapp/         - <The Application>
    .SYS/            - <Internal, Orion/OrionAPI only>
        run.sh*      - <Runs this Application and feeds data from conf.cfg to OrionAPI>
        conf.cfg     - <Metadata regarding this Application>
        INSTALL.sh   - <Ran by Orion on install>
        UNINSTALL.sh - <Ran by Orion on uninstall>
    libs/            - <Non-Native Libraries>
    static/          - <Application-wide Data Storage>
    data/            - <User-specific Data Storage>
        <username>/  - <A given user's data directory>
        ...          - <A directory for as many users are there are on the system>
    src/             - <Application Source>
    icon.png         - <AppIcon. Can also be under static/>
    MyOApp.oapp*     - <Binary> 
```
**[Files marked with a `*` are required by Orion. All directories are required]**