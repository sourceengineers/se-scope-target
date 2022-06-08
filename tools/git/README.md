# Git tools folder
This folder contains script to help manage the releases on github. 

## Add remotes
Before the release can be published, the remotes must be added locally to the repository. 
To do so, execute:
```bash 
./tools/git/init_remotes.sh ./
```

## Publish releases
To publish the release branch on all repositories, execute:
```bash 
./tools/git/publish_release.sh ./
```
Note, you still have to manage the ssh key setup on your own.
