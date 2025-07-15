# CS 247 Project: RAIInet \- Project Plan & Task Breakdown

- Balaji Leninrajan
- Kayla Chang
- Tyler Chen

## Core Architecture: Model-View-Controller (MVC)

As we decided, the entire application will be built on the MVC pattern, held together by the **Observer** pattern.

- **Model**: Contains the game's state and logic (`Game`, `Board`, `Player`, `Link`, `Ability` classes). It is completely independent of the UI.

- **View**: Displays the Model's state. We will implement `TextView` and `GraphicalView` classes. The Views will be **Observers** of the Model.

- **Controller**: Takes user commands, parses them, and updates the Model.

This architecture ensures that our game logic is decoupled from the user interface, which will make implementing the graphical view later much easier.

## Class Structure Breakdown

Here is the detailed breakdown of the classes we will implement.

- **`main.cpp`**

  - **Responsibility**: Entry point of the program. Parses command-line arguments (`-ability1`, `-link1`, `-graphics`, etc.). Initializes the `Game`, `Controller`, and `Views`, wires them together, and starts the main game loop in the `Controller`.

- **`Controller`**

  - **Responsibility**: Manages the main game loop, reads all user input (`move`, `ability`, `board`, etc.), and calls the appropriate methods on the `Game` (Model) object.

- **Model Classes**

  - **`Game`**

    - **Responsibility**: The central Model class. Holds the `Board` and `Player` objects. Manages turn progression and checks for win/loss conditions (a player downloading four data or four viruses).

    - **Potential Members**: `std::unique_ptr<Board> board`, `std::vector<Player> players, int currentPlayerIndex`.

  - **`Board`**

    - **Responsibility**: Represents the 8x8 game board. Manages the placement and position of all `Link` and `Firewall` objects. Validates move legality (e.g., staying on the board, not landing on friendly links or server ports).

    - **Pattern**: Can be implemented as a **Composite** of `Cell` objects. We can create a **Iterator** for cleanly traversing the board.

    - **Potential Members**: `std::vector<std::vector<Cell>> grid`.

  - **`Cell`**

    - **Responsibility**: Represents a single square on the board. Knows if it is a server port and holds a pointer to the `Link` or `Firewall` occupying it.

  - **`Player`**

    - **Responsibility**: Holds a player's collection of Links and `Ability` cards. Tracks downloaded data and virus counts.

    - **Potential Members**: `int dataDownloaded, int virusesDownloaded, std::vector<std::unique_ptr<Ability>> abilities, std::vector<Link*> links.`

  - **`Link` (Abstract Base Class)**

    - **Responsibility**: Interface for a game piece. Defines properties like strength, owner, and whether it's revealed. Will have virtual methods like `getType()` and `getStrength()`.

    - **Subclasses**: `VirusLink`, `DataLink`.

  - **`LinkDecorator` (Abstract Decorator)**

    - **Pattern**: The base class for the **Decorator** pattern. Inherits from `Link`.

    - **Concrete Decorator**: `LinkBoostDecorator`. This will wrap a `Link` and modify its movement behavior.

- **Ability Classes (Strategy Pattern)**

  - **`Ability` (Abstract Base Class)**

    - **Pattern**: The **Strategy** interface.

    - **Potential Method**: `virtual void use(Board& board, Player& user, Player& opponent, const std::vector<std::string>& params) = 0;`. Notice we are not passing the whole `Game` object to reduce coupling.

  - **Concrete Abilities**

    - `FirewallAbility, DownloadAbility, PolarizeAbility, ScanAbility, LinkBoostAbility.`

    - Each will implement the `use` method with its specific logic.

- **Factory Classes (Factory Pattern)**

  - **`LinkFactory` & `AbilityFactory`**

    - **Responsibility**: Creates sets of `Links` and `Abilitys` based on file input or command-line strings. This encapsulates the complex setup logic.

- **View Classes (Observer Pattern)**

  - **`View` (Abstract Base Class)**

    - **Responsibility**: The **Observer** interface.

    - **Key Method**: `virtual void update() = 0;, virtual void display() = 0;.`

  - **`TextView` & `GraphicalView`**

    - **Responsibility**: Concrete observers that implement `update` and `display`. When notified by the model, they will pull the new game state and render it to the console or a graphical window. The display must show the board, downloaded counts, and available abilities for the current player.

## Implementation Phases

We will build the project in logical, testable phases. This ensures we always have a working program, as recommended by the spec.

- **Phase 1: Core Foundation & Build System**

  - **Goal**: Create all class files (`.h/.cpp`) with basic definitions and method stubs.

  - **Tasks**:

    1. Set up the Makefile or CMakeLists.txt.

    2. Create all header files with class declarations.

    3. Create all `.cpp` files with empty method implementations.

    4. Ensure the project compiles into a runnable program that does nothing.

- **Phase 2: Text-Based Game Logic (The MVP)**

  - **Goal**: Implement a fully playable game with only the text interface.

  - **Tasks**:

    1. Implement the `Board` and `Cell` classes.

    2. Implement the `TextView` to draw an empty board.

    3. Implement the `LinkFactory` to create links from a file.

    4. Implement the `Player` and `Game` classes to manage state.

    5. Implement the `Controller` to parse `move`, `board`, and `quit` commands.

    6. Implement movement and battle logic (strength comparison, capturing).

    7. Implement win/loss conditions.

- **Phase 3: Adding Abilities**

  - **Goal**: Integrate the ability system.

  - **Tasks**:

    1. Implement the `Ability` Strategy interface and `AbilityFactory`.

    2. Implement each of the five required abilities as a concrete strategy class.

    3. Implement the `LinkBoostDecorator`.

    4. Update the `Controller` to handle the `abilities` and `ability <N> <params>` commands.

    5. Design and implement the three new custom abilities.

- **Phase 4: The Graphical View**

  - **Goal**: Add the graphical user interface.

  - **Tasks**:

    1. Choose a simple graphics library (e.g., X11).

    2. Implement the `GraphicalView` class as another Observer.

    3. Implement drawing logic for the board, links (with colors for type), and game status info.

    4. Handle the `-graphics` command-line flag.

- **Phase 5: Finalization & Polish**

  - **Goal**: Final bug fixes, documentation, and ensuring all spec requirements are met.

  - **Tasks**:

    1. Thoroughly test all commands and interactions.

    2. Implement robust error handling for bad commands.

    3. Implement the `sequence <file>` command.

    4. Write internal code documentation and review the project structure.

## Risk Management & Priorities

Following the spec's advice, our priority is to always have a working, submittable program.

- **Top Priority (MVP)**: Completing **Phase 2**. A program that allows two players to move pieces and battle in a text-only interface is a passing submission.

- **Secondary Priority**: Completing **Phase 3**. Adding the full ability system is the next most important feature set.

- **Final Priority**: Completing **Phase 4**. The graphical interface will be the last major component we add.

