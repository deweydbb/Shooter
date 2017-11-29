CHANGELOG START DATE 9/7/2017

FORMAT:
DATE
BLUEPRINT CHANGES:
C++ CHANGES:
ANY OTHER CHANGES:


9/13/17 8:03 David
BLUEPRINT CHANGES:
  Changed player movement to blueprint
  Added treehouse blueprint class
  Movment blueprints now allow player to climb the lader of the treehouse class
  
9/14/17 7:55 David
ANY OTHER CHANGES
  Added potential gun moddels and starter player animations

9/16/17
BLUEPRINT CHANGES:
  	Added main menu blueprints to thrid person character
ANY OTHER CHANGES: 
  	Created new folder in content called menuHUD and it includes the wiget for the main menu.
  	Press escape to bring up the menu, and two buttons appear: options and exit game
  	Curently options does nothing becasue we have no options.
  	Exit game when pressed closes the game.
  
9/17/17
BLUEPRINT CHANGES:
	Modified player blueprint so that the menu closes when escape is pressed.
ANY OTHER CHANGES:
  	Modified menu wiget so the buttons look less jenky

9/20/17
BLUEPRINT CHANGES:
  Beutified the menu and added shadows to the text

9/26/17
ANY OTHER CHANGES:
  Sam finally pulled with git lfs assets working.
  Use insttructions here:
	// Skip smudge
	git lfs install --skip-smudge
	
	// Fresh clone of repo (master branch)
	git clone my_repo_url

	// Switch to Testing branch
	git checkout Testing

	// Fetch lfs files
	git lfs fetch

	// Really get the files
	git lfs checkout

	// Fix smudge
	git lfs install --force

10/5/17
BLUEPRINT CHANGES:
	Added tree meshes to content folder.
	In separate project, tested the car meshes, scaled them to proportion and made them 
	into simple blueprint classes. Then migrated the car blueprints/meshes in to shooter
ANY OTHER CHANGES:
	Completed general organization of files for clarity

10/6/17
ANY OTHER CHANGES:
	Worked on the Charles character model in class using adobe fuse beta and added it to shooter.
	Specified bones to ue4 naming convention, and then modified base pose to "a pose"
	Copied ue4 basic ue4 animation blueprint class to charle skeleton.
	Mostly works except arms and hips and squeezed and it looks weird

10/7/17
ANY OTHER CHANGES:
	Tried to fix the charles animations but failed, I think it has to do with the type of bone used
	I need to specify that these bones are used for animation, rather than the default state

10/9/17
BLUEPRINT CHANGES:
	Attempted to add crouch, but instead of the head moving down to crouch, the head has a fixed position
	and the feet crouch up, so charles floats in the air when crouching
	It appears the error comes from the local position of the animations, but after trying
	to manually edit the crouch animations, I messed it up pretty bad.
	So I reimported both Charles and Bad Scholar and that seemed to fix the issue of the floating crouch

C++ CHANGES:
ANY OTHER CHANGES:	
	After some googling, I found out that i need to set the bones to recursive animation instead of
	simply animation. Arms and hips look normal and animation looks great.
	Added Bad Scholar and set bones to recursive animation and everything looks good

10/11/17
BLUEPRINT CHANGES:
	In the starter animations, I found another animation blueprint class that is closer to what 
	we will need for our game. I messed around a little bit, but the jump is terrible so I might try
	to combine the jump from the basic animation blueprint into this one

10/12/17
BLUEPRINT CHANGES:
	In class we fixed camera rotation in the player blueprint so the camera no longer spins around
	the character and is fixed behind his head. Also we began to add place holder guns onto the 
	characters with sockets. However the rotation of the guns is messed	

10/13/17
BLUEPRINT CHANGES:
	Worked to change the spawn locations for teams

10/15/17
BLUEPRINT CHANGES:
	The player blueprint was getting messy, so I reorgainized it into functions so it is
	less cluttered and more readable		

10/18/17 -David
BLUEPRINT CHANGES:
	Worked on action and mesh replication for clients. Got everything to work except the meshes
	So you can see people sprint, crouch, jump etc. but impossible to tell which team they
	are on.
ANY OTHER CHANGES:
	added the ability to aim vertically, through animation blueprints 

10/21/17
BLUEPRINT CHANGES:
	Changed the animations for aiming to ironsights, it will be easier to track and will add the
	ability for us to have a sniper with a scope.
	In class we attempted to fix the mesh replication by creating a game mode class.

10/22 - 11/9
ANY OTHER CHANGES:
	In attempting to fix mesh replications we broke the game. The editor wont open and crashed
	at 72% when attempting to open it. The weird thing was is that suddenly all of our commits
	wouldn't load, which made us believe it was a problem with the engine or visual studios, not
	something we had done in the actual blueprints, or c++ code. However, after much searching,
	googleling, stress, and dejection we disvocered that the gamemode class we had made was the 
	thing causing all the errors. Once we deleted the class, the editor opended and we hastily
	deleted all references to the class.

	Essentially between 10/22 and 11/9, Chanow and I spent about an hour a night, debugging and
	trying to find the error. We tried a lot of things, including re-installing unreal, and vs.
	So we don't have a lot of progress to show for this time, becuase we were frantically trying
	to get the program to run again.

11/11/17
BLUEPRINT CHANGES:
	Added sockets to the ends of the gun models so that we can track their location and fire
	projectiles from them, so the projectiles actually come out of the gun.

11/14/17
C++ CHANGES:
	In class we attempted to added projectiles to our game. We first tried to migrate from a
	progam chanow had already made, but becasue it was in first person, we ran into some errors
	like it not recognizing and object pointer becasue it was from an incomplete class even though
	the class was fully defined. However we did make some progress to adding projectiles and 
	working guns, as we made the actual material and particle class for the projectile. 			
	

11/16/17 - Sam Chanow
Blueprint Chnages:
	Instead of trying to merge the files from another project, we decided
	to start from scratch and derive a new shooter projectile blueprint class.
C++ Changes:
	Fixed errors regarding projectile class, but there are still some persisting errors in regards
	to firing projectiles as a whole 

11/19/17 THANKSGIVING BREAK
Blueprint Changes:
	Changed sprinting so it relies on stamina. Max stamina: 100, Deduction rate .5 every .05 seconds.
	Gain .5/3 stamina every .05 seconds. I added a widget that displays the stamina.

11/21/17 THANKSGIVING BREAK
Blueprint Changes:
	Worked to beautify the stamina bar. Also added placeholder health and magazine bars to the same wiget.
	Created a new user interface material to beautify the progress bars. Adapted from lazer material
	so it has a fade from top to bottom, but can still specify color of differing progress bars.

11/22/17 THANKSGIVING BREAK
C++ Changes:
	Got projectile basics working. Shoots out of body, not from muzzle. Could not get location of socket at the muzzle of the gun.
	Can still shoot while sprinting, crouching, etc

11/23/17 THANKSGIVING BREAK
Blueprint Changes:
	Worked more on projectiles, made a magazine class and mapped it to the widget progress bar
	so the player can see the number of bullets left

11/26/17 THANKSGIVING BREAK
Blueprint/C++ Changes:
	Added bool to control what state the player can fire in. The bool is set in the character blueprint.
	The player cannot shoot while: sprinting, jumping, climbing a ladder, or while in a menu.
Animation Changes:
	Made it so that the player aims up and down all the time except when sprinting, jumping, climbing, menuing, or crouching
	instead of just when aiming.

11/28/17
BluePrint Changes:
	Mapped the get health to the widget so it accurately displays the players health bar
C++ Changes:
	Added health property to players
	added remove health, get health, and add health functions
	Added projectile collision with characters
	Started working on projectile replication but it needs work