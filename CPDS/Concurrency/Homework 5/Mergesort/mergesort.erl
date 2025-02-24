%% Mergesort
%%
%% Students:
%%
%% Mehdi Hassanpour
%%

-module(mergesort).
-compile([export_all]).

%% 1.Seperation part
sep(L,0)        -> {[], L};
sep([H|T], N)   -> {Lleft, Lright} = sep(T,N-1), {[H|Lleft],Lright}. 

%% 2.Merging part
merge([],L2) -> L2;
merge(L1,[]) -> L1;
merge([H1 | T1], [H2 | T2]) when H1 =< H2 -> [H1 | merge(T1, [H2 | T2])];
merge([H1 | T1], [H2 | T2]) when H1 > H2 -> [H2 | merge([H1 | T1], T2)].

%% 3.Mergesort -- Sequential
ms([])  -> [];
ms([A]) -> [A];
ms(L)   -> {L1,L2} = sep(L, length(L) div 2), merge(ms(L1),ms(L2)).

%% 4.Mergesort -- Parallel version
rcvp(Pid) -> 	
	receive
		{Pid, L} -> L
	end.

pms(L) ->	Pid = spawn(msort, p_ms, [self(), L]),
    		rcvp(Pid).

p_ms(Pid, L) when length(L) < 100 -> Pid ! {self(), ms(L)};
p_ms(Pid, L) -> {Lleft, Lright} = sep(L, length(L) div 2),
                Pid1 = spawn(msort, p_ms, [self(), Lleft]),
                Pid2 = spawn(msort, p_ms, [self(), Lright]),
                L1 = rcvp(Pid1),
                L2 = rcvp(Pid2),
                Pid ! {self(), merge(L1,L2)}.
