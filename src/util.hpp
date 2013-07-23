#ifndef SX_LOAD_TX_HPP
#define SX_LOAD_TX_HPP

#include <bitcoin/bitcoin.hpp>

using namespace bc;

bool load_tx(transaction_type& tx, const std::string& filename)
{
    std::ifstream infile(filename, std::ifstream::binary);
    if (!infile)
    {
        std::cerr << "showtx: Bad file." << std::endl;
        return false;
    }
    std::ostringstream contents;
    contents << infile.rdbuf();
    infile.close();
    data_chunk raw_tx = decode_hex(contents.str());
    try
    {
        satoshi_load(raw_tx.begin(), raw_tx.end(), tx);
    }
    catch (end_of_stream)
    {
        std::cerr << "showtx: Deserializing transaction failed." << std::endl;
        return false;
    }
    return true;
}

bool read_private_key(elliptic_curve_key& key)
{
    std::istreambuf_iterator<char> it(std::cin);
    std::istreambuf_iterator<char> end;
    std::string privkey(it, end);
    secret_parameter secret = wif_to_secret(privkey);
    if (secret == null_hash || !key.set_secret(secret))
    {
        std::cerr << "Invalid private key." << std::endl;
        return false;
    }
    return true;
}

#endif

