#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

// Max voters and candidates
#define MAX_VOTERS 100
#define MAX_CANDIDATES 9

// preferences[i][j] is jth preference for voter i
int preferences[MAX_VOTERS][MAX_CANDIDATES];

// Candidates have name, vote count, eliminated status
typedef struct
{
    string name;
    int votes;
    bool eliminated;
}
candidate;

// Array of candidates
candidate candidates[MAX_CANDIDATES];

// Numbers of voters and candidates
int voter_count;
int candidate_count;

// Function prototypes
bool vote(int voter, int rank, string name);
void tabulate(void);
bool print_winner(void);
int find_min(void);
bool is_tie(int min);
void eliminate(int min);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: runoff [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates
    candidate_count = argc - 1;
    if (candidate_count > MAX_CANDIDATES)
    {
        printf("Maximum number of candidates is %i\n", MAX_CANDIDATES);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i].name = argv[i + 1];
        candidates[i].votes = 0;
        candidates[i].eliminated = false;
    }

    voter_count = get_int("Number of voters: ");
    if (voter_count > MAX_VOTERS)
    {
        printf("Maximum number of voters is %i\n", MAX_VOTERS);
        return 3;
    }

    // Keep querying for votes
    for (int i = 0; i < voter_count; i++)
    {

        // Query for each rank
        for (int j = 0; j < candidate_count; j++)
        {
            string name = get_string("Rank %i: ", j + 1);

            // Record vote, unless it's invalid
            if (!vote(i, j, name))
            {
                printf("Invalid vote.\n");
                return 4;
            }
        }

        printf("\n");
    }

    // Keep holding runoffs until winner exists
    while (true)
    {
        // Calculate votes given remaining candidates
        tabulate();

        // Check if election has been won
        bool won = print_winner();
        if (won)
        {
            break;
        }

        // Eliminate last-place candidates
        int min = find_min();
        bool tie = is_tie(min);

        // If tie, everyone wins
        if (tie)
        {
            for (int i = 0; i < candidate_count; i++)
            {
                if (!candidates[i].eliminated)
                {
                    printf("%s\n", candidates[i].name);
                }
            }
            break;
        }

        // Eliminate anyone with minimum number of votes
        eliminate(min);

        // Reset vote counts back to zero
        for (int i = 0; i < candidate_count; i++)
        {
            candidates[i].votes = 0;
        }
    }
    return 0;
}

// Record preference if vote is valid
bool vote(int voter, int rank, string name)
{
    // First, check all candidates for a name match between candidate and ballot
    for (int candidateindex = 0; candidateindex < candidate_count; candidateindex++)
    {
        if (strcmp(name, candidates[candidateindex].name) == 0)
        {
            // Update the preference array
            preferences[voter][rank] = candidateindex;
            return true;
        }
    }
    return false;
}

// Tabulate votes for non-eliminated candidates
void tabulate(void)
{
    // Check every voter's ballot
    for (int voterindex = 0; voterindex < voter_count; voterindex++)
    {
        // For that voter's ranked preferences
        for (int rankindex = 0; rankindex < candidate_count; rankindex++)
        {
            // If the candidate is eliminated in that voters ranked preferences slot, then move to the next ranked choice.
            if (candidates[preferences[voterindex][rankindex]].eliminated)
            {
                continue;
            }
            // If the candidate is not eliminated, add a vote to that candidate. Then stop the loop.
            else
            {
                candidates[preferences[voterindex][rankindex]].votes++;
                break;
            }
        }
    }
    return;
}

// Print the winner of the election, if there is one
bool print_winner(void)
{
    // First, define the winning number of votes. This would be 50% of the vote + 1 to gain a majority.
    int winning_total = (voter_count / 2) + 1;
    // Check all candidates vote totals
    for (int totalindex = 0; totalindex < voter_count; totalindex++)
    {
        // Any candidate with a vote score equal or higher than the winning total is the winner, so print their name.
        if (candidates[totalindex].votes >= winning_total)
        {
            printf("%s\n", candidates[totalindex].name);
            return true;
        }
    }
    return false;
}

// Return the minimum number of votes any remaining candidate has
int find_min(void)
{
    // First, define a variable which is the minimum votes, set it to the highest possible vote.
    int minimumvote = voter_count;
    // Now, we must search every non eliminated candidates vote counts, updating minimumvote to be the lowest value.
    for (int candidateindex = 0; candidateindex < candidate_count; candidateindex++)
    {
        if (candidates[candidateindex].eliminated)
        {
            continue;
        }
        else if (candidates[candidateindex].votes < minimumvote)
        {
            minimumvote = candidates[candidateindex].votes;
        }
    }
    return minimumvote;
}

// Return true if the election is tied between all candidates, false otherwise
bool is_tie(int min)
{
    // Define a boolean that can be updated checking if the tie is true or not
    bool equaltomin = false;
    // Iterate through each candidate and check if they match min
    for (int candidateindex = 0; candidateindex < candidate_count; candidateindex++)
    {
        // Eliminated candidates are skipped over.
        if (candidates[candidateindex].eliminated)
        {
            continue;
        }
        // If any candidate has any vote value different to min, break the loop.
        else if (candidates[candidateindex].votes != min)
        {
            equaltomin = false;
            break;
        }
        // Otherwise, considering these two cases, the candidates still have votes equal to min, for now the bool is true
        else
        {
            equaltomin = true;
        }

    }
    return equaltomin;
}

// Eliminate the candidate (or candidates) in last place
void eliminate(int min)
{
    //Check each candidate once again
    for (int candidateindex = 0; candidateindex < candidate_count; candidateindex++)
    {
        //first, don't consider any already eliminated candidates
        if (candidates[candidateindex].eliminated)
        {
            continue;
        }
        //Otherwise, find the candidate (or candidates) with the minimum votes and eliminate them.
        else if (candidates[candidateindex].votes == min)
        {
            candidates[candidateindex].eliminated = true;
        }
    }
    return;
}