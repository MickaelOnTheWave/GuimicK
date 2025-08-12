# GuimicK

A set of tools to schedule and run recurrent tasks on your computer, even on your absence.
The usages that motivated it were :
- Running backups periodically and automatically
- Running system updates
- Running antivirus scans
- Having a report sent to the user detailing what happened to each task after each run

## Usage

A tutorial is available on how to download/install it on Windows : 
https://mickaelcguimaraes.wixsite.com/guimick/services

## How to build it

## Limitations

### Maintenance
This software is old and has not been maintained for some years. It has only recently been uploaded to GitHub and migrated to Cmake.
As such, it as been untested for a while and some features might be broken.

### Feedback to user
It was originally developed around the idea of sending an email to the user with a report of all tasks. Although the code still works, today
most email providers (at least gmail, which I used at the time) don't allow anymore external SMTP access. As such, the email reporting feature is now
useless, at least for my personal usage.
A Telegram bot has been added later, but I didn't finish the development of it. It has some basic features but full production runs were not tested.

### Automated tests
There were several automated tests developed around QTest. I migrated some of them to Catch2 and moved to test/ folders, however all of the main tests
on external backup services haven't been migrated yet. Their code is still present in oldSrc/ and oldInclude/ for a future migration.

## Platforms

It was first designed to run on Linux, and then generalized for Windows. Mac is untested.
It runs on both platforms, although not all features are available everywhere :

Tested on Windows only :
- Setting scheduled runs directly from the configuration tool

Tested on Linux only :
- Waking remote machines and taking control of them
- Shutting down automatically


