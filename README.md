🌦️ My Weather App (C++)

A terminal-based Weather Application built in C++ that fetches real-time weather data using the OpenWeatherMap API.
The project focuses on clean architecture, proper error handling, and an interactive CLI user interface.

🚀 Features
🌍 Check real-time weather of any city
🔁 Re-check weather without returning to main menu
⚖️ Compare weather between two cities
🕒 Maintain user search history (file-based)
🎨 Colorful terminal UI with animations
❌ Proper API & network error handling

🛠️ Tech Stack
Language: C++
Networking: libcurl
API: OpenWeatherMap
Platform: Linux / Android (CxxDroid)
Storage: File handling (fstream)

📸 Preview (CLI) 

https://github.com/CodeVerseAnkit/My-Weather-App/blob/main/example2.jpg
https://github.com/CodeVerseAnkit/My-Weather-App/blob/main/example%201.jpg

⚙️ How It Works
User enters city name
App sends request using libcurl
Receives raw JSON response
Extracts required values manually
Formats and displays weather data
Stores searched cities in a history file

🔑 API Configuration
This project uses OpenWeatherMap API.
Create a free account on OpenWeatherMap
Generate your API key
Replace the API key in the source code

⚠️ Note:
For production-level projects, API keys should not be hardcoded.

❗ Error Handling
The app gracefully handles:
Invalid city names
Network issues
Invalid API keys
Too many requests (Rate limiting)
Unknown server errors
All errors are mapped using a strongly typed enum class ApiError.

📌 Known Limitations
Manual JSON parsing (string-based, not library-based)
API key is currently hardcoded
Forecast data not included (current weather only)

🔮 Future Improvements
Use a proper JSON library (nlohmann/json)
Store API key in config/environment
Improve modular UI structure

👨‍💻 Author
Ankit
B.Sc. Computer Application Student
Learning C++ • DSA • System Design

⭐ Support
If you find this project helpful, consider giving it a ⭐ on GitHub.
Feedback and suggestions are always welcome.
