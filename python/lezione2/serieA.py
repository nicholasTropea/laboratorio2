#! /usr/bin/env python3

import sys

def printTeamStats(name, points, scored, taken):
    print(f"{name}: Punti: {points} Gol segnati: {scored} Gol subiti: {taken}")

def updateScoreboard(sboard, team1, score1, team2, score2):
    # Calculate points
    if score1 > score2:
        points1 = 3
        points2 = 0
    elif score2 > score1:
        points1 = 0
        points2 = 3
    else:
        points1 = 1
        points2 = 1
    
    # Update team2 stats
    if team1 in sboard:
        stats = sboard[team1]
        stats[0] += points1
        stats[1] += score1
        stats[2] += score2
    else:
        sboard[team1] = [points1, score1, score2]
    
    # Update team2 stats
    if team2 in sboard:
        stats = sboard[team2]
        stats[0] += points2
        stats[1] += score1
        stats[2] += score2
    else:
        sboard[team2] = [points2, score1, score2]


def createScoreboard(fileName):
    sboard = {}

    for line in fileName: # Iterates trough file lines
        a = line.split() # Splits when whitechars are found (if no split parameters are passed)

        if len(a) == 0: # Empty line
            continue 

        if len(a) != 4: # Wrong line
            raise RuntimeError(f"Linea non corretta: {line}")

        score1 = int(a[0])
        score2 = int(a[1])
        team1 = a[2]
        team2 = a[3]

        # Dictionaries are passed by reference
        updateScoreboard(sboard, team1, score1, team2, score2)
    
    return sboard

def main(fileName):
    # also:
    #   f = open(fileName, "r")
    #   ...
    #   f.close()

    # Preferred way of using files
    with open(fileName, "r") as f: # The instructions that use the file MUST be inside the with block
        sboard = createScoreboard(f)
    
    # print(sboard) printing dictionaries is ugly

    teams = list(sboard) # Creates a list with a dictionary keys

    teams.sort(key=lambda x: [sboard[x][0], sboard[x][1] - sboard[x][2]], reverse=True)
    # Sorts by:                   points           goal difference

    for team in teams:
        printTeamStats(team, sboard[team][0], sboard[team][1], sboard[team][2])


# Start the program
if len(sys.argv) == 2:
    main(sys.argv[1])
else:
    print(f"Uso:\n\t{sys.argv[0]} fileName_risultati")