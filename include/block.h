/**
 * Abstract base class for block implementation
 */

#ifndef BLOCK_H
#define BLOCK_H

#include <string>

class Block {
public:
    virtual ~Block() = default;
    virtual std::string getHash() const = 0;
    virtual std::string getPreviousHash() const = 0;
    virtual int getIndex() const = 0;
    virtual std::string calculateHash() const = 0;
    virtual void display() const = 0;
};

#endif