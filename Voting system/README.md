# Online Voting System Readme

## Overview
This repository contains the source code for an Online Voting System implemented in C++. The system allows users to register, log in, update their profiles, give votes to candidates, display voting results, and view personal details.

## Features
1. **Registration:** Users can register with a unique username and password. The system validates the username's uniqueness and ensures that the user is at least 18 years old.

2. **Login/Logout:** Registered users can log in and out of the system using their username and password.

3. **Update Profile:** Users can update their password and name after logging in.

4. **Give Vote:** Users can cast votes for candidates. Once a user has voted, they cannot vote again.

5. **Display Result:** The system displays the voting results, showing the number of votes each candidate received.

6. **View Personal Details:** Users can view their personal details, including username, name, date of birth, voting status, and voter ID.

7. **Exit:** Users can exit the program, and the voter data is saved to a file before termination.

## File Structure
- **main.cpp:** The main source code file containing the implementation of the online voting system.
- **voter_data.txt:** File to store voter data.
- **personal_details.txt:** File to store personal details of voters.
- **vote_details.txt:** File to store details of votes cast.

## Usage
1. Clone the repository to your local machine.
2. Compile the `main.cpp` file using a C++ compiler.
3. Run the compiled executable to start the program.
4. Follow the on-screen menu to interact with the online voting system.

## Dependencies
- C++ compiler

## Notes
- The program uses files (`voter_data.txt`, `personal_details.txt`, `vote_details.txt`) to store and retrieve data between sessions.
- Sample candidates ("Candidate A," "Candidate B," "Candidate C") are provided in the code. You can customize and add more candidates as needed.

## Contributing
If you'd like to contribute to this project, feel free to fork the repository and submit pull requests.

## License
This project is licensed under the [MIT License](LICENSE). Feel free to use, modify, and distribute the code.
