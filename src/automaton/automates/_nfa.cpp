#include "automates/_nfa.hpp"

#include <istream>

namespace automates
{

_nfa::_nfa(const uint32_t states, const uint32_t start, std::vector<uint32_t> finals, std::vector<uint32_t> tr)
    : finite_automaton(states, std::unique(tr.begin(), tr.end()) - tr.begin() - 1, start),
      m_final_states(std::move(finals)), m_trans_table(std::move(tr))
{
    for (const auto &fs : m_final_states)
        assert(fs <= m_states && "Incorrect final state");
    assert(m_trans_table.size() == m_states * m_states && "Incorrect size of the transition table");
    for (const auto &tf : m_trans_table)
        assert(tf <= m_alphabet && "Incorrect symbol in transition table");
}

bool _nfa::is_final(uint32_t st) const noexcept
{
    const auto &fs = m_final_states;
    return std::find(fs.begin(), fs.end(), st) != fs.end();
}

void _nfa::print(std::ostream &out) noexcept
{
    out << "\tNFA Automaton\n";
    for (uint32_t st = 0; st < m_states; ++st, out << '\n')
    {
        if (st == m_start)
            out << "->";
        out << st;
        if (is_final(st))
            out << "*";
        out << "\t|";

        for (uint32_t i = 0; i < m_states; ++i, out << '\t')
            out << m_trans_table[st * m_states + i];
    }
}

_nfa construct_read(std::istream &in)
{
    uint32_t states, start,
            fs_num;
    std::vector<uint32_t> table, final_states;
    in >> states >> start;

    in >> fs_num;
    final_states.reserve(fs_num);
    table.reserve(states*states);

    for (; fs_num; --fs_num)
    {
        uint32_t x;
        in >> x;
        final_states.push_back(x);
    }

    for (auto i = table.capacity(); i; --i)
    {
        uint32_t x;
        in >> x;
        table.push_back(x);
    }

    return _nfa(states, start, std::move(final_states), std::move(table));
}

} // namespace automates