//
// Name: memory
// Project: Cache
// Author: Shen Sijie
// Date: 5/8/18
//

#ifndef CACHE_MEMORY_H
#define CACHE_MEMORY_H

#include <stdint.h>
#include "storage.h"

class Memory : public Storage {
public:
    Memory() {}

    ~Memory() {}

    // Main access process
    void HandleRequest(uint64_t addr, int bytes, int read,
                       char *content, int &hit, int &time);

private:
    // Memory implement

    DISALLOW_COPY_AND_ASSIGN(Memory);
};

#endif //CACHE_MEMORY_H
