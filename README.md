# Crowd behavior. Fire
Simple imitation of crowd behavior in case of fire.
## How it looks:
![image](https://user-images.githubusercontent.com/52485539/63609649-b080aa80-c601-11e9-85db-eb6955daba13.png)
## Symbols:
* ![image](https://user-images.githubusercontent.com/52485539/63609928-5d5b2780-c602-11e9-8997-55e268161da2.png) - Human
* ![image](https://user-images.githubusercontent.com/52485539/63610759-71078d80-c604-11e9-8799-138e53c090bf.png) - Dead human
* ![image](https://user-images.githubusercontent.com/52485539/63610065-b7f48380-c602-11e9-87d9-d37a08ffdac6.png) - Fire extinguisher
* ![image](https://user-images.githubusercontent.com/52485539/63610120-d3f82500-c602-11e9-92ef-efe2376ab4b5.png) - Evacuation button
* ![image](https://user-images.githubusercontent.com/52485539/63610247-2cc7bd80-c603-11e9-86bd-826248f428c2.png) - “Exit” pointer
* ![image](https://user-images.githubusercontent.com/52485539/63610302-5b459880-c603-11e9-8abf-a497df0f45ce.png) - Smoke
* ![image](https://user-images.githubusercontent.com/52485539/63610374-8f20be00-c603-11e9-80af-3a702167a39b.png) - Fire
* ![image](https://user-images.githubusercontent.com/52485539/63610546-ec1c7400-c603-11e9-826d-ee0c949ece3c.png) - Wall
* ![image](https://user-images.githubusercontent.com/52485539/63610659-271ea780-c604-11e9-822d-890c9138c183.png) - Exit
## What's going on here:
The model simulates the behavior of a crowd in a confined space in the event of a fire.
Fire extinguishers, evacuation buttons and “Exit” pointers are randomly placed in the room.
Pointers and buttons are placed only against walls.
People come into the room who randomly move around it and after a while go out.
After a random period of time, a fire occurs in the room, fire and smoke appear, or only smoke.
Fire and smoke begin to spread throughout the room. A person has a radius of visibility.
If he sees fire or smoke, he begins to leave the room. 
If at the same time he sees the evacuation button, then he goes to it and presses it.
After clicking, all the people in the room go to the exit.
If a person sees the “Exit” pointer, he approaches him and looks at him,
after which he knows how to get to the exit and will easily leave the building in cases of falling into a smoky space.
If a person sees a fire extinguisher and has sufficient courage, he takes a fire extinguisher, moves to the source of ignition.
If he sees a fire, he begins to extinguish it; if he does not see it, he goes to the exit. In the immediate vicinity of the fire,
a person receives burns from which he may die. If a person enters the smoke, and he does not see the pointer,
then he is lost and begins to move into random neighboring cells, also in the smoke he suffocates and after some time can die.
## Requirements:
* QtCreator 4.9.1 installed.
## How to use:
* Just download all files.
* Run /build-Qt_Practice_2019-Desktop_Release/release/Qt_Pracrice_2019.exe
or make your build with copy walls.dat in your build directory
* Click start button and watch
* When all people die or are successfully evacuated, close window.
* You can change the location of the walls in the room. Use walls.dat file and update room button

