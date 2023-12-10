
# Pong Game

To showcase the strengths of our operating system, we have implemented the classic game, Pong, as our app. In this game, two players control paddles to bounce a ball back and forth. 

## Features
- **Clear Entry Instructions:** Upon starting, players receive concise instructions about controls and the time limit, ensuring an even playing field for all.
- **Multi-button controller support:** Our multi-button controller support allows for two-player gameplay. The CMD button also serves to pause the game.
- **Game timer:** The game timer adds an exciting element, challenging players to outdo each other in shorter timeframes.
- **Pause Functionality (Video interrupt):** Using the OS's video interrupt feature, the game can be paused and resumed seamlessly, enhancing the intensity of play.
- **Engaging Graphics:** Our physics engine brings the game to life with responsive paddles and a ball that reacts realistically upon impacts. The pause icon and changing background further enrich the visual experience.

## Game Control and Rules
In our Pong game, players aim to block the ball with their paddles and score against the opponent. One player can control their paddle using the 'w' and 'x' keys of the multi-button controller, while the other player uses the 'u' and 'k' keys for control. The ball's speed increases with each hit, adding to the challenge. The physical simulation engine varies the ball's speed based on the hit location on the paddle, mimicking real-world physics. The CMD button on the controller is used for pausing and resuming the game. A timer adds urgency to the gameplay, with a visual cue when it reaches halfway. When the timer counts down to zero, the game will be ended.


## Getting Started
### Simulator Files
Be sure that you run the latest simulator files that is provided on the professor's [repository](https://github.com/UCDClassNitta/riscv-console). You can also get it with this commend:
```
git clone git@github.com:UCDClassNitta/riscv-console.git
```
### Docker Image
Make sure you are running the latest docker version for the RISC-V Docker image `cjnitta/riscv_base`. If you are not, you should remove the docker image and run the command:
```
./rvconsole.sh
```
to get the latest image.

### Launch Docker Container
Change directories into the riscv-console and run the console launch script with the command:
```
./rvconsole.sh
```
This will mount the current riscv-console directory into the `/code` directory of the container allowing for source files to be edited on the host machine as well as within the container. Once the container is launched you should see a prompt like:
```
root@fedcba9876543210:/code#
```

### Compile the Firmware and Cartridge
The firmware is located in `/code/firmware` folder. You can compile it with
```
make clean && make
```

**Note:** If you are running ported versions, you don't have to compile. The compiled file is already included. 

The cartridge is located in `/code/cartridge` folder. You can compile it with
```
make clean && make
```

### Launch Simulator
You can launch the simulator with the full path using the command:
```
/code/runsim.sh
```
You can now insert the firmware with the file located in `/code/firmware/bin/riscv-console-example.strip`

Once inserted the firmware, you can click on the `Run` button to run the operating system. Next, you can insert the cartridge that is located in `/code/cartridge/bin/riscv-console-example.strip`. After that, our Pong game will be started!
