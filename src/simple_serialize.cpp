#include <simple_serialize.hpp>

std::ostream &operator<<(std::ostream &os, const raft::Entry &entry) {
  return os << entry.info.index << " " << entry.info.term << " " << entry.data;
}

std::istream &operator>>(std::istream &is, raft::Entry &entry) {
  is >> entry.info.index >> entry.info.term >> entry.data;
  return is;
}

std::ostream &operator<<(std::ostream &os,
                         const raft::RPC::AppendEntriesRequest &request) {
  os << "AppendEntriesRequest " << request.term << " " << request.leader_id
     << " " << request.prev_log_index << " " << request.prev_log_term << " "
     << request.leader_commit << " " << request.entries.size() << " ";
  for (auto &entry : request.entries) {
    os << entry << " ";
  }
  os << "\n";
  return os;
}

std::istream &operator>>(std::istream &is,
                         raft::RPC::AppendEntriesRequest &request) {
  size_t num_entries;
  is >> request.term >> request.leader_id >> request.prev_log_index >>
      request.prev_log_term >> request.leader_commit >> num_entries;
  for (size_t i = 0; i < num_entries; ++i) {
    raft::Entry entry;
    is >> entry;
    request.entries.emplace_back(std::move(entry));
  }
  return is;
}

std::ostream &operator<<(std::ostream &os,
                         const raft::RPC::AppendEntriesResponse &request) {
  return os << "AppendEntriesResponse " << request.peer_id << " "
            << request.term << " " << request.success << " "
            << request.match_index << " "
            << "\n";
}

std::istream &operator>>(std::istream &is,
                         raft::RPC::AppendEntriesResponse &request) {
  return is >> request.peer_id >> request.term >> request.success >>
         request.match_index;
}

std::ostream &operator<<(std::ostream &os,
                         const raft::RPC::VoteRequest &request) {
  return os << "VoteRequest " << request.term << " " << request.candidate_id
            << " " << request.last_log_index << " " << request.last_log_term
            << " "
            << "\n";
}

std::istream &operator>>(std::istream &is, raft::RPC::VoteRequest &request) {
  return is >> request.term >> request.candidate_id >> request.last_log_index >>
         request.last_log_term;
}

std::ostream &operator<<(std::ostream &os,
                         const raft::RPC::VoteResponse &response) {
  return os << "VoteResponse " << response.peer_id << " " << response.term
            << " " << response.vote_granted << " "
            << "\n";
}

std::istream &operator>>(std::istream &is, raft::RPC::VoteResponse &response) {
  return is >> response.peer_id >> response.term >> response.vote_granted;
}

std::ostream &operator<<(std::ostream &os,
                         const raft::RPC::ClientRequest &request) {
  return os << "ClientRequest " << request.data << " "
            << "\n";
}

std::istream &operator>>(std::istream &is, raft::RPC::ClientRequest &request) {
  return is >> request.data;
}

std::ostream &operator<<(std::ostream &os,
                         const raft::RPC::ClientResponse &response) {
  return os << "ClientResponse " << response.error_message << " "
            << response.entry_info.index << " " << response.entry_info.term
            << " " << response.leader_id << " " << response.leader_info.ip
            << " " << response.leader_info.client_port << " "
            << "\n";
}

std::istream &operator>>(std::istream &is,
                         raft::RPC::ClientResponse &response) {
  return is >> response.error_message >> response.entry_info.index >>
         response.entry_info.term >> response.leader_id >>
         response.leader_info.ip >> response.leader_info.client_port;
}