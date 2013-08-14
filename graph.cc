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

// ########################### endpt_begin ###########################
unordered_set<GraphEdge>::const_iterator DFSGraph::endpt_begin()
{
  return _endpts.cbegin();
}

// ########################### endpt_end #############################
unordered_set<GraphEdge>::const_iterator DFSGraph::endpt_end()
{
  return _endpts.cend();
}

// ########################### _dfs ##################################
void DFSGraph::_dfs(Tensor *t)
{
  // add to "unprocessed" list
  _vertices.insert(t);

  for(size_t i = 0; i < t->inputs(); ++i)
    {
      // process all tensors linked via inputs and outputs
      Tensor *adj = t->input_tensor(i);
      if(nullptr != adj)
	{
	  // Record the discovered edge, and recurse into the adjacent
	  // tensor if it has not been previously encountered.
	  if( !_vertices.count(adj) ) _dfs(adj);
	  _edges.insert(GraphEdge{t, i, adj, t->input_num(i)});
	}
      else _endpts.insert(GraphEdge{t, i, nullptr, 0});
    }

  for(size_t i = 0; i < t->outputs(); ++i)
    {
      // If the adjacent tensor has not yet been encountered, register
      // the edge and recurse into it.  The edge has been / will be
      // recorded when processing in the opposite direction.
      Tensor *adj = t->output_tensor(i);
      if(nullptr != adj)
	{
	  if(!_vertices.count(adj)) _dfs(adj);
	}
      else _endpts.insert(GraphEdge{nullptr, 0, t, i});
    }
}
