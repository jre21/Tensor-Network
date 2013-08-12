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

class Tensor;

class Graph
{
public:
  virtual ~Graph() {}
  // Reindex all tensors in the graph.  This must be called if the
  // connections between tensors are changed.
  virtual void rebuild() = 0;
  // The following four functions are iterators for use with functions
  // which need to act on either all tensors or all links.  No
  // guarantees are made about ordering.
  virtual std::iterator<std::input_iterator_tag, Tensor*> tensor_begin() = 0;
  virtual std::iterator<std::input_iterator_tag, Tensor*> tensor_end() = 0;
  virtual std::iterator<std::input_iterator_tag, pair<Tensor*,Tensor*> >
    vertex_begin() = 0;
  virtual std::iterator<std::input_iterator_tag, pair<Tensor*,Tensor*> >
    vertex_end() = 0;
};

class ConcreteGraph : public Graph
{
public:
  // Create the graph of all tensors connected to t.
  Graph(Tensor *t);
  ~Graph() {}
  // From interface Graph.
  void rebuild() override;
  std::iterator<std::input_iterator_tag, Tensor*> tensor_begin() override;
  std::iterator<std::input_iterator_tag, Tensor*> tensor_end() override;
  std::iterator<std::input_iterator_tag, pair<Tensor*,Tensor*> >
    vertex_begin() override;
  std::iterator<std::input_iterator_tag, pair<Tensor*,Tensor*> >
    vertex_end() override;
protected:
  // function which is called recursively to find all connected
  // tensors via depth-first search
  void _dfs(Tensor *t);
private:
  // tensors which have not yet been processed
  unordered_set<Tensor*> discovered;
  // tensors which belong to the graph
  unordered_set<Tensor*> nodes;
  // vertices connecting tensors
  unordered_set<pair<Tensor*,Tensor*> > vertices;
};
