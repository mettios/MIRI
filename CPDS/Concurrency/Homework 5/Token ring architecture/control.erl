%% Token ring architecture (Control)
%%
%% Students:
%%
%% Mehdi Hassanpour
%%

-module(control).
-export([go/2]).

% Start the game: spawn the workers, create a ring, and send the first token.
go(N, M) when N >= 1, M > 0 ->
    flush_mailbox(),
    TargetList = generate(M),
    io:format("Target List: ~p~n", [TargetList]),
    Workers = worker_ring(N, self()),  % Set up the worker ring
    [FirstWorker | _] = Workers,
    FirstWorker ! token,  % Start the game by sending the token
    ResultList = control_game(TargetList, []),
    io:format("Result List: ~p~n", [ResultList]).

% Generate a list of M random numbers in range 1..M
generate(0) -> [];
generate(M) -> [rand:uniform(M) | generate(M-1)].

% Flush the mailbox to clean obsolete messages
flush_mailbox() ->
    receive
        _Any -> flush_mailbox()
    after
        0 -> ok
    end.

% Control loop: handle workers {Pid, eat} messages and collect results
control_game([], ResultList) ->
    ResultList;
control_game(TargetList, ResultList) ->
    receive
        {Pid, eat} ->
            [H | T] = TargetList,
            io:format("~p eats~n", [Pid]),
            control_game(T, [{Pid, H} | ResultList])
    end.

% Create a ring of N workers and return a list of their PIDs
worker_ring(1, PidControl) ->
    FirstWorker = spawn(worker, worker_node, [PidControl]),
    FirstWorker ! FirstWorker,  % Connect the single worker to itself
    [FirstWorker];
worker_ring(N, PidControl) ->
    FirstWorker = spawn(worker, worker_node, [PidControl]),
    connect_workers(N-1, PidControl, FirstWorker, FirstWorker, []).

connect_workers(0, _PidControl, FirstWorker, LastWorker, Workers) ->
    LastWorker ! FirstWorker,  % Close the ring by connecting the last worker to the first
    [LastWorker | Workers];
connect_workers(N, PidControl, FirstWorker, PrevWorker, Workers) ->
    NewWorker = spawn(worker, worker_node, [PidControl]),
    PrevWorker ! NewWorker,  % Connect previous worker to the new one
    connect_workers(N-1, PidControl, FirstWorker, NewWorker, [PrevWorker | Workers]).
