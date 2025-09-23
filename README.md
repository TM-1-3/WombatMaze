<img src='https://sigarra.up.pt/feup/pt/imagens/LogotipoSI' width="30%"/>

<div align="center">
🌍 <a href="README.md">English</a> | 🇵🇹 <a href="README.pt.md">Português</a>
</div>

<h3 align="center">BSc in Informatics and Computing Engineering<br>L.EIC009 - Computer Laboratory<br> 2024/2025 </h3>

---
<h3 align="center"> Collaborators &#129309 </h2>

<div align="center">

| Name             | Number      |
|------------------|-------------|
| Bárbara Gomes    | up202305089 |
| Francisco Leiras | up202307823 |
| Tiago Yin        | up202306438 |
| Tomás Morais     | up202303834 |

Grade : 18,0

</div>

# Wombat Maze Project Report

- [What was our goal? What is our application?](#goal)
- [Project Structure](#structure)
- [Implemented Devices](#devices)  
   - [The i8254 PC’s Timer](#timer)  
   - [The i8042 PC’s Keyboard](#keyboard)  
   - [The i8042 PC’s Mouse](#mouse)  
   - [Video Card](#video)
- [What are the differentiating features of our project?](#features)

<a id="goal"></a>
## What was our goal? What is our application?

The objective of our project was to develop an interactive and visually engaging game
within the `LCOM/Minix` environment, making use of at least four different I/O devices
studied throughout the semester, specifically, the `keyboard`, `mouse`, `timer`, and `graphics card`.
Our primary aim was to create a complete application that combined gameplay mechanics,
user interaction, and device integration in a cohesive and entertaining experience.

The result is **Wombat Maze**, a dynamic 2D game where the player controls a wombat
character who must traverse through three unique maps: a **forest**, **outer space**, and the **beach**.
Each map presents increasing levels of difficulty, with more obstacles and a faster and more
deadly enemy. The player must navigate each environment using the `keyboard`, while
interacting with and destroying obstacles using the `mouse`. Meanwhile, a `timer` counts down
from 60 time units, and a pursuing enemy, represented by a monster, actively chases the
wombat whenever it has a line of sight.

The core gameplay loop is centered around **survival, navigation, and quick
decision-making**. The enemy can pass through obstacles, unlike the wombat, which adds to
the urgency and strategy of obstacle destruction. The game ends in either **victory**, when the
wombat completes all three maps before time runs out, or **defeat**, if the `dingoe` catches the
wombat or the timer expires. The application also includes a **main menu** with options to start
the game, view instructions, or exit, enhancing usability and user experience.

In essence, **Wombat Maze** merges creativity and technical execution to deliver a fun,
responsive game that highlights practical application of systems programming concepts,
while maintaining an enjoyable player experience.

<a id="structure"></a>
## Project Structure

The project's structure has been organized in a modular way to facilitate maintenance and
scalability. The `main.c` file contains the application's main loop and is responsible for
controlling the execution cycle, processing inputs and updating the screen.

The central `src` module coordinates the other components: `display` handles graphic
rendering with the support of `sprite.c`, `io` manages communication with the hardware (such as
the timer, keyboard, mouse and video), and `game` brings together the game's main logic.

The game logic is divided into various files, such as `wombat.c`, `dingoe.c`, `obstacle.c`,
`maze.c` and `menu.c`, each responsible for different elements of the game, such as characters,
character collisions, obstacles and menus.

<div align="center">
<img width="907" height="597" alt="Captura de ecrã de 2025-09-23 10-55-53" src="https://github.com/user-attachments/assets/e87bb972-b712-49de-bcd8-368b15885d91" />

***Architecture Diagram of the Project***
</div>

<a id="devices"></a>
## Implemented Devices

<a id="timer"></a>
### The i8254 PC’s Timer

The `Timer 0` was used to generate periodic interrupts at a frequency of `30Hz`, basically
regulating and processing all in-game events, at this given rate, like refreshing the screen,
drawing elements, updating positions, checking for collisions etc.

The implementation of the Timer was mostly notable through the inclusion of an in-game
timer inside the levels, which was programmed to decrease by one unit every two real-time
seconds.

In order to successfully include the `i8254` PC’s Timer in the project, the functions  
`timer_subscribe_int`, `timer_unsubscribe_int`, `timer_set_frequency` and
`timer_int_handler` were implemented and used.

<a id="keyboard"></a>
### The i8042 PC’s Keyboard

The Keyboard was implemented to process key presses and releases, via interrupts.
These actions, depending on the current game state, are used to control the `Wombat`,
navigate through the menu (using the `WASD` or `Arrow` keys), select options and exit screens
and maps (using the `ESC` or `ENTER` key).

In order to successfully include the `i8042` PC’s Keyboard in the project, the functions  
`keyboard_subscribe_int`, `keyboard_unsubscribe_int` and `kbc_ih` were implemented and
used.

<a id="mouse"></a>
### The i8042 PC’s Mouse

The Mouse was configured to process the pressing of its left button and movement, via
interrupts.

This device is present in the entirety of the game, its use is only relevant inside the levels,
where the player can make the Wombat destroy the obstacles in its way, by moving the cursor
over the obstacle and then pressing the Mouse’s left button.

In order to successfully include the `i8042` PC’s Mouse in the project, the functions  
`mouse_subscribe_int`, `mouse_unsubscribe_int` and `mouse_ih` were implemented and used.

<a id="video"></a>
### Video Card

The video card was configured to use **direct color mode** with a screen resolution of
`800x600` pixels and `8 bits` per RGB color component (`32 bits` in total). To eliminate flickering,
**Double Buffering** was implemented.

In order to successfully include the Video Card in the project, the functions  
`set_mode_graphic`, `build_frame_buffer` and `swap_buffers` were implemented and used.

To display graphic elements on the screen, a class `Sprite` was created, which defines the
position to draw it, as well as storing the element’s bitmap. Sprites are loaded from **XPM** files
and converted to bitmap using the `xpm_load` function, provided by the `LCF`. The color
`#FFFFFE` was used as a transparency key and skipped during sprite rendering.

In order to successfully include the displaying of sprites in the project, the functions  
`loadSprite`, `drawSprite`, `destroySprite` and `draw_pixel` were implemented and used.

<a id="features"></a>
## What are the differentiating features of our project?

Our game introduces several gameplay innovations. One of the key features is the enemy’s
behavior: the monster will pursue the wombat when it gets to its line of sight. This adds a
layer of strategy, as players must carefully consider how and when to move, taking advantage
of obstacles for cover or to break line of sight.

Another unique mechanic is the **interactive obstacle destruction system**. While the wombat
cannot pass through obstacles, players can use the mouse to remove them in real time. This
gives the player an active role in shaping the game environment by managing `keyboard`
movement and `mouse`-based destruction.

The game also features **progressive difficulty** across levels. As players advance through
the maps, the monster moves faster and obstacles become more frequent, requiring more
strategic planning. The inclusion of a countdown timer intensifies the experience, putting
players under constant time pressure to complete objectives before either the monster catches
them or the time runs out.

**Wombat Maze** also offers creative **visual themes and environments**. Rather than using a
single static map, the game takes the player through three unique settings — a forest, outer
space, and a beach. Each environment has its own visual identity, layout design, and
gameplay feel, providing a refreshing change of pace and atmosphere as the player
progresses.
