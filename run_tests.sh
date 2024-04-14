#!/bin/bash

total_tests=61
tests_to_run=1
color_reset='\033[0m'  # Reset color
color_prompt='\033[1;36m'  # Cyan color for prompts
color_test='\033[1;32m'  # Green color for test messages

for num in $(seq 2 $total_tests); do
    # Format the number to have leading zeros if needed
    test_num=$(printf "%03d" "$num")

    # Print the current test number in green color
    echo -e "${color_test}Running test ${test_num}...${color_reset}"

    # Run valgrind command
    valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./build/funcc ../cs429h_s24_p4__tests/${test_num}.fun > output

    # Decrement the number of tests to run without prompts
    tests_to_run=$((tests_to_run - 1))

    # Check if tests_to_run is zero, then prompt the user
    if [ $tests_to_run -eq 0 ]; then
        echo -e "${color_prompt}Press Enter to continue, enter the number of tests to run without prompts, or press q to quit...${color_reset}"

       read -r tests_input

       # Check if the input is empty (just Enter)

        if [ -z "$tests_input" ]; then
            tests_to_run=1
        elif [ "$tests_input" == "q" ]; then
            echo -e "${color_prompt}Stopping...${color_reset}"
            break
        else
            tests_to_run=$tests_input
        fi
    fi
done