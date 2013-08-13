// Copyright 2013 Jacob Emmert-Aronson
// This file is part of Tensor Network.
//
// Tensor Network is free software: you can redistribute it and/or
// modify it under the terms of the GNU General Public License as
// published by the Free Software Foundation, either version 3 of the
// License, or (at your option) any later version.
//
// Tensor Network is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Tensor Network.  If not, see
// <http://www.gnu.org/licenses/>.

#pragma once

#include <iterator>
#include <unordered_set>

// forward declare to avoid dependencies between headers
class Tensor;

// struct to represent the edges of a graph
struct GraphEdge
{
  Tensor *input_tensor;
  size_t input_num;
  Tensor *output_tensor;
  size_t output_num;
  // test equality of elements independent of order
  bool operator== (const GraphEdge &v) const;
  bool operator!= (const GraphEdge &v) const;
};

// Define hash function overload for GraphEdge.
template <>
struct std::hash<GraphEdge>
{
  size_t operator() (const GraphEdge &v) const
  {
    // Just XOR the element hashes together.
    std::hash<Tensor*> Thash;
    std::hash<size_t> Ihash;
    return Thash(v.input_tensor) ^ Ihash(v.input_num)
      ^ Thash(v.output_tensor) ^ Ihash(v.output_num);
  }
};

// A graph maps all the tensors reachable from a specific tensor as
// well as the edges connecting them.  If the links connecting a
// tensor are changed, graphs containing that tensor are no longer
// valid.
class Graph
{
public:
  virtual ~Graph() {}
  // Number of vertices and edges
  virtual size_t vertices() = 0;
  virtual size_t edges() = 0;
  // The following four functions are iterators for use with functions
  // which need to act on either all tensors or all links.  No
  // guarantees are made about ordering.
  virtual std::unordered_set<Tensor*>::const_iterator vertex_begin() = 0;
  virtual std::unordered_set<Tensor*>::const_iterator vertex_end() = 0;
  virtual std::unordered_set<GraphEdge>::const_iterator
  edge_begin() = 0;
  virtual std::unordered_set<GraphEdge>::const_iterator
  edge_end() = 0;
};

class DFSGraph : public Graph
{
public:
  // Create the graph of all tensors connected to t.
  explicit DFSGraph(Tensor *t);
  DFSGraph& operator=(const DFSGraph&) = default;
  DFSGraph(const DFSGraph&) = default;
  DFSGraph& operator=(DFSGraph&&) = default;
  DFSGraph(DFSGraph&&) = default;
  ~DFSGraph();
  // From interface Graph.
  size_t vertices() override;
  size_t edges() override;
  std::unordered_set<Tensor*>::const_iterator vertex_begin() override;
  std::unordered_set<Tensor*>::const_iterator vertex_end() override;
  std::unordered_set<GraphEdge>::const_iterator edge_begin() override;
  std::unordered_set<GraphEdge>::const_iterator edge_end() override;
protected:
  // function which is called recursively to find all connected
  // tensors via depth-first search
  void _dfs(Tensor *t);
private:
  // tensors which have not yet been processed
  std::unordered_set<Tensor*> _discovered;
  // tensors which belong to the graph
  std::unordered_set<Tensor*> _nodes;
  // vertices connecting tensors
  std::unordered_set<GraphEdge> _vertices;
};
