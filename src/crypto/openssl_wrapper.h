#pragma once

#include <array>
#include <utility>

#include <openssl/evp.h>

namespace shadow
{

namespace network
{

namespace crypto
{

class CryptoProvider
{
public:
    /**
     *  Diffie Hellman algorithms return keys sizes in bytes.
     *  NOTE: Only metadata.
     */
    struct dh_return_keys_size_bytes_t
    {
        dh_return_keys_size_bytes_t() = delete;
        static constexpr uint8_t X25519_ret_size_b = 32;
    };

public:
    using public_key_t = std::array<uint8_t, dh_return_keys_size_bytes_t::X25519_ret_size_b>;
    using private_key_t = public_key_t;

public:
    static std::pair<public_key_t, private_key_t> get_pub_priv_keys()
    {
        using ret_val_t = std::invoke_result_t<decltype(get_pub_priv_keys)>;

        /* Return value */
        ret_val_t ret_val {};

        /* Allocates public key algorithm context */
        EVP_PKEY_CTX* ctx = EVP_PKEY_CTX_new_id(EVP_PKEY_X25519, nullptr);

        /* Initializes the public key algorithn context for a key generation operation */
        EVP_PKEY_keygen_init(ctx);

        /* Generate the public key + private */
        EVP_PKEY* pkey = nullptr;
        EVP_PKEY_keygen(ctx, &pkey);

        size_t len_pub = sizeof(ret_val.first);
        EVP_PKEY_get_raw_public_key(pkey, ret_val.first.data(), &len_pub);

        size_t len_priv = sizeof(ret_val.second);
        EVP_PKEY_get_raw_public_key(pkey, ret_val.second.data(), &len_priv);

        return ret_val;
    }
};

}
}
}