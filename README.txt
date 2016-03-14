Question 1, Each member's contribution:
Runzhong Zhang: initial working AI, initial heuristic table (beat Simple Player), non-recursive version minimax, pass the minimax test, mobility function
Matthew: recursive minimax, alpha-beta pruning
Together: end-game solver, design a good heuristic function

Question2, Document the improvement of AI:
We implemented minimax with alpha-beta pruning, which should give a speed advantage over plain minimax. To have a strong heuristic scoring function, we used a heuristic value table from the Internet with some improvements, and a mobility score. We played versions of our AI with and without mobility against each other, and the AI with mobility won easily, so our AI with mobility should be stronger than other AIs with just heuristic tables. We tried to also use frontier calculation in our heuristic scoring, but that did not seem to help the AI play better.