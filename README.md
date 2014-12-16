CPSC 426 Final Project  
December 2014  
Hugh O'Cinneide  

# Chordõs (Chord-ish)

An implementation of some of the ideas presented in the Chord paper.

A client can create a new distributed hash table, or join another one. Once in
a DHT network, the client can search for files shared to the network and upload
its own files.

### Usage

Start a new client by first making a Makefile with `qmake-qt4`. Then you can
`make` it, and run the `chord` binary produced.

Once you have a client up, start a DHT by typing in `New` to the command field.
Join an existing DHT by giving the hostname:portnumber to the command field.

Upload a file using the `Share File` button. Search for shared files by typing
your query into the search bar, and results will appear below it.

When finished, be sure to type `Quit` into the command bar to gracefully leave
the network.

### Simplifying Assumptions

It was surprisingly difficult for me to devise a way to implement remote
procedure calls and remote reads using synchronous messaging. Instead, I made
every node be aware of the network topology, which takes out everything novel
presented in the original chord paper.

Another simplification is that I did not use full 160-bit SHA-1 hashes to assign
names and hashes, instead I wrote a function `Sha1Mod64`, which does what it
says on the box, to hash blocks and nodes into `quint64`s which are much easier
to deal with. 

### Implementation

Chord-ish was implemented almost from scratch, only using a little boilerplate
code from peerster.

### Motivation

### Goals

### Improvements
