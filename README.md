# BooM
A hash map using the [BBHash](https://github.com/rizkg/BBHash) perfect hash function.

## Motivation

Often I've wanted a very fast and compact map backed by a minimal perfect hash.  This is for a scenario where one cares more about the space and time requried for querying than the time required to build the map.  Often, however, perfect hash function construction is very slow, requires a lot of extra memory, or both. The recent BBHash library from [@rizkg](https://github.com/rizkg) changes that, and provides a minimal perfect hash function that can be built *very fast* using *very little* extra memory.  This library simply provides a "Map" wrapper around the BBHash hash function that allows one to associate values with the keys.  Here, I'm most interested in a scenario where one can query the map with *any* key (i.e. including ones on which the PHF was not constructed).  Thus, we have to keep the keys and values around to allow proper queries to our map. BooM (short for BooPHF-Map --- BBHash used to be called BooPHF) tries to wrap all of this up nicely, and also lets you serialize your map to and from disk.

## Dependencies

Currently depends on the [cereal](http://uscilab.github.io/cereal/) library to support serialization of the map to / from disk.

## TODO

* Talk to @rizkg about how the hasher works so that we properly support the user passing in their own hasher to the map.
* Make the interface richer (more than just `find()`, `save()` and `load()`).
