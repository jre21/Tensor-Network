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

#include <functional>
#include "graph.hh"
#include "tensor.hh"

using std::hash;
using std::iterator;
using std::unordered_set;

// ########################### GraphEdge ################################
// ########################### operator== ############################
bool GraphEdge::operator== (const GraphEdge &v) const
{
  return input_tensor == v.input_tensor && input_num == v.input_num
    && output_tensor == v.output_tensor && output_num == v.output_num;
}

// ########################### operator!= ############################
bool GraphEdge::operator!= (const GraphEdge &v) const
{
  return input_tensor != v.input_tensor || input_num != v.input_num
    || output_tensor != v.output_tensor || output_num != v.output_num;
}


// ########################### DFSGraph ##############################
// ########################### constructor ###########################
DFSGraph::DFSGraph(Tensor *t)
{
  _dfs(t);
}

// ########################### vertices ##############################
size_t DFSGraph::vertices()
{
  return _vertices.size();
}

// ########################### edges #################################
size_t DFSGraph::edges()
{
  return _edges.size();
}

// ########################### vertex_begin ##########################
unordered_set<Tensor*>::const_iterator DFSGraph::vertex_begin()
{
  return _vertices.cbegin();
}

// ########################### vertex_end ############################
unordered_set<Tensor*>::const_iterator DFSGraph::vertex_end()
{
  return _vertices.cend();
}

// ########################### edge_begin ############################
unordered_set<GraphEdge>::const_iterator DFSGraph::edge_begin()
{
  return _edges.cbegin();
}

// ########################### edge_end ##############################
unordered_set<GraphEdge>::const_iterator DFSGraph::edge_end()
{
  return _edges.cend();
}

// ########################### _dfs ##################################
void DFSGraph::_dfs(Tensor *t)
{
}
