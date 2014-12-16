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

When a node uses the `New` command to make a new DHT, a new Peer object is
created to describe itself. Peers are identified by the SHA-1 mod 64 hash of the
48-bit identifier `IP << 16 + port`.  When a node joins a DHT, it indicates one
node already inside the DHT to collect data from. It asks the current member of
the DHT for all the current nodes in the DHT, along with all the files shared.
The requested node sends it back a short description of all the nodes in the
network along with information about all the files shared and their block list
hashes. The joining node then requests blocks from the node that comes after it
on the hash circle, and stores any blocks it receives. The network is
represented by a circularly linked list of Peer objects inside each node.

When a node shares a file, it breaks the file up into BLOCKSIZE (default 8000)
byte blocks, takes their hash, and assigns them to the appropriate node on the
network based on that hash. When a node receives a block, it saves it to disk in
a file `./client_downloads/{Node Name}/{Block Name}`. The file's name and block
list hash is then broadcast out to all the nodes in the DHT.

When a node wants to download a file, it finds the file in its local map of file
names to block list hashes, requests the block list from the appropriate node,
and then repeats the process for each of the block hashes.

When a node leaves gracefully, it notifies all the nodes on the network to prune
the node from their view of the network, and then gives all its files to the
next node. 

### Next steps

There are a few next steps with this project:

* Handling non-graceful exits with redundant storage and network correctness
  checks
* Recursively breaking up files into Merkle trees to make sure we aren't sending
  files larger than BLOCKSIZE over the network
