# InventoryTest
A test click and drag inventory system with 2D and 3D pick up items, and a short simple dialogue test. This was for a test assignment for a game developer contract application.

-----------Assignment Tasks----------
1. Inventory System
Implement a basic pickup system where objects placed in the world can be added to the inventory after the player interacts with them.

Create an Inventory UI with 4 item slots and a viewport slot.

Implement a drag-and-drop mechanism between slots and the viewport slot.

When an item is dragged to the viewport slot of the inventory:

If it’s a 2D item, display a larger version.

If it’s a 3D item, display the 3d model in the world inside the widget. Implement logic to control the rotation and zooming of the shown object.

Include a close button and ESC key logic to hide the inventory.

2. Simple Dialogue Trigger
Place an interactable actor in the world.

When interacted with, trigger a dialogue box with a short message.

The dialogue box should be possible to be moved forward, each skip displaying the next segment of text.

When displaying the final part, the player should be able to close the dialogue box.

-----------Testing Features----------

The project can be opened in an Unreal Engine 5.4 Editor. Movement of the character is done by the WASD keys, you can control the camera with the mouse, and you can jump with the space bar.

Upon playing, the inventory menu will open up, you can exit this by pressing escape or by left-clicking the exit button on the top left. There are various pickups on the ground that you can collect, and as you do, they will enter your inventory. If your active inventory slot on the top right is empty, the item will be placed there. When your inventory is full, you can no longer pick up items. 3D items will scale and rotate appropriately to fit the slot and face the camera.

To open the inventory, press the I key on the keyboard. From here, you will see both 3D and 2D items. If you click and drag an item, you can move it around (there is a bug at the moment where the focus isn't initially set upon opening the inventory, so to move items, you need to click anywhere on the screen once first). Placing items in an empty space will move it, in an occupied space will swap items, and in the spot on the bottom right corner will forget the item. Any item placed in the large slot will remain on screen on the top right in the "active inventory slot" when closing the inventory.

To showcase the dialogue system in the game, you can walk up to any of the two characters on the map, and when you're close enough in front of them, you can press the E key on the keyboard to activate the dialogue. The speech bubble will appear above the head of the current speaker. To advance the dialogue, press E again. When the dialogue is complete, the last bubble will disappear, and the player can move again.

-----------Design Decisions----------

I decided to use the default third-person level Unreal has as a base. I also decided to use C++ both because that is what I am most comfortable with, and because I feel it is easier to debug and build larger systems with (I would say performance reasons as well, but that isn't as important in a smaller assignment like this). The widgets I created can be found in Content/UI, and the entirety of the C++ classes are mine, with the exceptions being InventoryTest, InventoryTestGameMode, and the bulk of InventoryTestCharacter (minus the dialogue bubble aspects I added and the control mode changes)

For the inventory system, I set up the UMG widgets and handled their logic in the C++ parent classes. InventoryTestPlayerController handles initializing the inventory object (which manages the mechanical aspects of the inventory) and the HUD. The HUD houses the inventory widgets and manages the visuals of the 2D Objects placed in them, as well as handling the transform of 3D objects. The inventory slots are all UBorder widgets with an image under them. That image is replaced as needed when objects move to match the object that should be in that slot. If the object is 3D, the image will remain empty, and the 3D object will be placed in the position of the slot using projection logic between the world and the viewport. The image (ignoring 3D objects) that is being dragged is actually a duplicate created image that only exists during the dragging process. I set this up this was so that I didn't have to worry about handling changing the position of the images in the widget hierarchy. I just simply have to track the textures, where they go, and then change them as necessary. The PlayerInventory at the time of moving items is updated to actually track the item and their data in the slots. I use mouse update events to track when the mouse picks up, drops, and drags an object. I use its screen position to determine the slot it is hovering over and if it is hovering over the forget area.

The dialogue system is rather simple. For a larger project, I would rather use a dialogue plug-in to set up a proper dialogue system (I used NotYet in my most recent Unreal project to a T, which you can find my use of it on my portfolio page). That way, I could easily handle branching dialogue paths, responding to questions, etc. But for this smaller project, I decided to keep it simpler by making a simple DialogueManager class. A DialogueTrigger holds an array of dialogue nodes. At the moment, these are simple, just the speaker and the text for them to say. When the player enters a DialogueTrigger and presses E, the dialogue trigger sends that info to the dialogue manager and starts the dialogue, displaying the first bubble. From there, the player controller tracks input to tell the manager when to advance dialogue, and when the manager reaches the end of the array, it ends the dialogue event. The dialogue bubbles use widget components attached to each character to display the speech bubble in screen space, but above each character's head, even as the camera moves.

‐-------------Things I plan to fix/polish------------

- I want to clean up the code for inventory management, PlayerHUD seems a bit too messy for my liking and I feel there isn't enough of a standard as to which class handles what (there's some widget and image handling in PlayerInventory when that's supposed to purely be mechanical, and there's some function calls that call back to a class that's already above it on the call stack).

- I want to make the rotation and scaling of the 3D objects more accurate. The items don't fit the entire slot as well as the 2D items do, and they rotate oddly with the perspective of the camera when moving a 3D item to the edge of the viewport

- I want to add controls to use right click and mouse wheel to control the scale and rotation of a 3D item in the active inventory slot when the inventory is open

- Set the 3D items to render over everything (UI as well) when in the inventory. I was originally looking into using RenderTargets to handle this, but was worried that that may be overcomplicating it. I'm aware of how to render the mesh over other meshes, but if there isn't a simple way to render it over UI, I may reconsider this. If anything, it would simplify the system a lot since it naturally will fit with the use of images, as the 2D items do

- I'm aware of a drag-and-drop feature in Unreal already. I was going to look further into it, but felt more comfortable creating the system I had in mind. In hindsight, this may have been reinventing the wheel a bit, and I'd like to see if it is a more optimal system

- Add extra polish like more widget animations and touch up smaller parts of the code to be cleaner, more optimal, and more comprehensible
