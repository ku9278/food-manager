# Food Manager

## Manage Your Refrigerator Efficiently
Food Manager is a C++ program designed to help you track and manage foods in your refrigerator, ensuring nothing goes to waste before its expiration date.

## Requirements
* c++ compatible compiler

## Installation
1. Clone the repository
   ```sh
   git clone https://github.com/ku9278/CPP2409-P.git
   ```
2. Build the program using your preferred C++ compiler

## Using the Application
1. Obtain an OpenAI API key at https://platform.openai.com/api-keys
2. Run food_manager.exe
3. Select language
4. Enter your OpenAI API key
5. Navigate through the menu and select the desired feature

### Features
1. View current foods
2. Add a food
3. Remove a food
4. View foods nearing expiration
5. View recommended menu
6. Exit program

#### View Recommended Menu
The program uses OpenAI's GPT-4o-mini model to analyze the foods in your refrigerator and provide creative meal ideas, helping you make the best use of your ingredients.

## Libraries Used
* [openai-cpp](https://github.com/olrea/openai-cpp)
* [curl](https://curl.se/)
