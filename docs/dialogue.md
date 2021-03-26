# Dialogue

The system used for communicating with various characters.

The dialogue system is not interactable, it simply displays text and characters in an order, like a cheap power point.

## The CSV File Format

The dialogue system uses CSV files to organize it's data. The CSV file may be generated with a program such as [Word](https://www.microsoft.com/en-us/microsoft-365/word) or [LibreOffice](https://www.libreoffice.org/).
It is suggested to NOT separate by space when loading and saving the CSV file.

### Commands

Every row is a dialogue command that follows the pattern `<TYPE> <DATA>`.

Type | Definition 
-- | --
LEFT | Set the texture to be used by the left stage character.
RIGHT | Set the texture to be used by the right stage character.
MESSAGE | The message that should be displayed. (waits until user continues)
SNIPPET | Run a snippet of wren code.
SCRIPT | Run a wren script.

### Character Portraits

Character portraits are automatically loaded from `res/textures/dialogue/<CHARACTER_EMOTION>.png` with the name `<CHARACTER_EMOTION>`. Note the capitalization. 

### Example

Type | Data
-- | --
LEFT | IAN_IDLE
RIGHT | FRANK_IDLE
MESSAGE | Hello, World!
SNIPPET | System.print(“Hello, World!”)
RIGHT | SAM_IDLE
MESSAGE | I can't believe this is working<br />And this will be written in a new line. 
SCRIPT | res/scripts/camera_shake.wren 
RIGHT | FRANK_ANGRY 
MESSAGE | WHAT WAS THAT!!?!?! 

#### The CSV File

The actual CSV file of the above example would be the following.

```csv
LEFT,IAN_IDLE
RIGHT,FRANK_IDLE
MESSAGE,"Hello, World!"
SNIPPET,"System.print(“Hello, World!”)"
RIGHT,SAM_IDLE
MESSAGE,"I can’t believe this is working!
And that is how we switch lines!"
SCRIPT,res/scripts/camera_shake.wren
RIGHT,FRANK_ANGRY
MESSAGE,"WHAT WAS THAT!!?!?!"
```

