Game:
Objective - Destroy as many asteroid as possible before you run out of lives to get a highscore.
It is a luck based and skill based game.
Game mechanics
Wave - Each wave spawns 5 more obsticles(asteroid / blackhole / whitehole) at a maximum of 100 obsticles.
Whitehole will spawn every 5 rounds while black hole will spawn every 10 rounds.
Starting from wave 10, every wave ending with 1 will increase the maximum size of the spawning asteroid while those ending with 6 will
increase the minimum size of the asteroid spawned.
Each spawns with a random size, position and velocity outside the gamescreen but the velocity would always point towards the gamescreen.
Asteroids will get a new postion and velocity once they go out of the spawn range outside the gamescreen.
Bullets will despawn if they go beyond the gamescreen while the ship will warp around the screen if that happens.
Blackhole and whiteholes will despawn once the go beyond the spawn range outside the gamescreen.
Asteroids will reduce in size if they hit other asteroids or bullets and will despawn if their size is less than 1.
Asteroid + Asteroid - Bigger Asteroid size - Smaller Asteroid size and despawn smaller astroid
(if both have the same size both will despawn).
Asteroid + Bullet - Asteroid size - Bullet damage.
Bullets are despawned upon hitting a asteroid or sucked into a blackhole.
A maximum of 100 bullets can be fired at once and no more can be fired before any is despawned.
At 100 score, bullets gains a homing function and will move towards the nearest asteroid from them.
At 250 score, bullets fire twice as fast.
Every 100 score, bullets deal 1 additional damage(1 before 100, 2 after 100, 3 after 200...).
Blackhole atrracts everything within range. Their attraction range and attraction force depends on their size.
Whitehole repels everything within range. Their repellent range and repellent force depends on their size.
Blackhole can devour other blackholes and whiteholes to increase in size.
Blackhole + Blackhole - Blackhole size + blacohole size and blackhole velocity + blackhole velocity. One of the blackhole will despawn.
Blackhole + Whitehole - Blackhole size * Whitehole size and blackhole velocity - Whitehole velocity. The whitehole will despawn.
The repellent force of a whitehole is greater than the attraction force of a blackhole of the same size 
to make it more difficult for a blackhole to devour a whitehole
If the ship hits an asteroid, the player will lose one life and the asteroid will despawn.
If the ship touches a blackhole, the game is over.

Mainmenu:
Spacebar to continue.
Esc - End game.
Gamemenu:
Esc - End game.
W - Move upwards.
A - Move left.
S - Move downwards.
D - Move right.
W + D - Move towards up right.
W + A - Move towards up left.
S + D - Move towards down right.
S + A - Move towards down left.
W + S / A + D - Nothing.
Q / E - Switch weapon type - Weapon type not working so it is not included.
P - Pause.
F - Testmode.

Pause:
Esc - End game.
P -Unpause.

Testmode:
Esc - End game.
F - Gamemode - Continue from wave with same no of asteroid, blackhole, whitehole but at different position and velocity.
G - Godmode - HP will not decrease.
P - Pause.
R - Spawn 25 asteroid of size 1 to 3 at random position with random velocity.
T - Spawn 25 blackhole of size 1 to 3 at random position with random velocity.
Y - Spawn 25 whitehole of size 1 to 3 at random position with random velocity.
Z - Spawn 1 blackhole at specified position with no velocity.
X - Spawn 1 whitehole at specified position with no velocity.
C - clear screen of all gameobject but ship.
V - Set HP to 3 and revive if dead.
I - Increase score by 100.
O - Decrease score by 100.

Gameover menu:
ESC -End game.