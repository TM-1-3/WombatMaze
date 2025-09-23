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

## What was our goal? What is our application?

The objective of our project was to develop an interactive and visually engaging game
within the LCOM/Minix environment, making use of at least four different I/O devices
studied throughout the semester, specifically, the keyboard, mouse, timer, and graphics card.
Our primary aim was to create a complete application that combined gameplay mechanics,
user interaction, and device integration in a cohesive and entertaining experience.

The result is Wombat Maze, a dynamic 2D game where the player controls a wombat
character who must traverse through three unique maps: a forest, outer space, and the beach.
Each map presents increasing levels of difficulty, with more obstacles and a faster and more
deadly enemy. The player must navigate each environment using the keyboard, while
interacting with and destroying obstacles using the mouse. Meanwhile, a timer counts down
from 60 time units, and a pursuing enemy, represented by a monster, actively chases the
wombat whenever it has a line of sight.

The core gameplay loop is centered around survival, navigation, and quick
decision-making. The enemy can pass through obstacles, unlike the wombat, which adds to
the urgency and strategy of obstacle destruction. The game ends in either victory, when the
wombat completes all three maps before time runs out, or defeat, if the dingoe catches the
wombat or the timer expires. The application also includes a main menu with options to start
the game, view instructions, or exit, enhancing usability and user experience.

In essence, Wombat Maze merges creativity and technical execution to deliver a fun,
responsive game that highlights practical application of systems programming concepts,
while maintaining an enjoyable player experience.

## Project Structure

The project's structure has been organized in a modular way to facilitate maintenance and
scalability. The main.c file contains the application's main loop and is responsible for
controlling the execution cycle, processing inputs and updating the screen.

The central src module coordinates the other components: display handles graphic
rendering with the support of sprite.c, io manages communication with the hardware (such as
the timer, keyboard, mouse and video), and game brings together the game's main logic.

The game logic is divided into various files, such as wombat.c, dingoe.c, obstacle.c,
maze.c and menu.c, each responsible for different elements of the game, such as characters,
character collisions, obstacles and menus.

<div align="center">
<img width="907" height="597" alt="Captura de ecrã de 2025-09-23 10-55-53" src="https://github.com/user-attachments/assets/e87bb972-b712-49de-bcd8-368b15885d91" />


**Architecture Diagram of the Project**
</div>

Implemented Devices
The i8254 PC’s Timer
 The Timer 0 was used to generate periodic interrupts at a frequency of 30Hz, basically
regulating and processing all in-game events, at this given rate, like refreshing the screen,
drawing elements, updating positions, checking for collisions etc.
 The implementation of the Timer was mostly notable through the inclusion of an in-game
timer inside the levels,which was programmed to decrease by one unit every two real-time
seconds.
 In order to successfully include the i8254 PC’s Timer in the project, the functions
timer_subscribe_int, timer_unsubscribe_int, timer_set_frequency and
timer_int_handler were implemented and used.
The i8042 PC’s Keyboard
 The Keyboard was implemented to process key presses and releases, via interrupts.
 These actions, depending on the current game state, are used to control the Wombat,
navigate through the menu (using the WASD or Arrow keys), select options and exit screens
and maps (using the ESC or ENTER key).
 In order to successfularily used to move a cursor inside the game screen, however, although
this element is presly include the i8042 PC’s Keyboard in the project, the functions
keyboard_subscribe_int, keyboard_unsubscribe_int and kbc_ih were implemented and
used.
The i8042 PC’s Mouse
 The Mouse was configured to process the pressing of its left button and movement, via
interrupts.
 This device is priment in the entirety of the game, its use is only relevant inside the levels,
where the player can make the Wombat destroy the obstacles in its way,by moving the cursor
over the obstacle and then pressing the Mouse’s left button.
 In order to successfully include the i8042 PC’s Mouse in the project, the functions
mouse_subscribe_int, mouse_unsubscribe_int and mouse_ih were implemented and used.

