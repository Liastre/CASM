// =============== DESCRIPTION ===============
// Created on 05 Jun.
// ===========================================

#ifndef CROSSAUDIOSTREAMMANAGER_BUFFER_BASE_INL
#define CROSSAUDIOSTREAMMANAGER_BUFFER_BASE_INL

template<typename T>
BufferTemplate<T>::BufferTemplate() {
    size = 0;
    filled = 0;
}

template<typename T>
BufferTemplate<T>::BufferTemplate(uint32_t paramSize) : BufferTemplate<T>::BufferTemplate() {
    size = paramSize;
    buffer.reserve(size);
}

template<typename T>
BufferTemplate<T>::~BufferTemplate() {
}

template<typename T>
bool BufferTemplate<T>::writeSingle(void* ptr) {
    if(filled == size) {
        return false;
    }
    buffer.emplace_back(*static_cast<T*>(ptr));
    filled++;

    return true;
}

template<typename T>
bool BufferTemplate<T>::writeVector(void* ptr, uint32_t paramSize) {
    if (filled+paramSize>size) {
        return false;
    }
    std::vector<T> array = *static_cast<std::vector<T>*>(ptr);
    buffer.insert(buffer.end(), array.begin(), array.end());
    filled += paramSize;

    return true;
}

template<typename T>
bool BufferTemplate<T>::writeArray(void* arrayPtr, uint32_t paramSize) {
    if (filled+paramSize>size) {
        return false;
    }
    T* array = static_cast<T*>(arrayPtr);
    for(uint32_t i=0; i<paramSize; i++) {
        buffer.emplace_back(array[i]);
    }
    filled += paramSize;

    return true;
}

template<typename T>
bool BufferTemplate<T>::writeArrayTemplate(void* arrayPtr, uint32_t arraySize, uint8_t typeSize) {
    if (filled+arraySize>size) {
        return false;
    }

    if(arrayPtr==nullptr) {
        for(uint32_t i = 0; i<arraySize*typeSize; i++) {
            buffer.emplace_back(0);
        }
        filled += arraySize*typeSize;

        return true;
    }

    bitsRep value;
    switch(typeSize) {
    case 1: {
        writeArray(arrayPtr, arraySize);
        break;
    }
    case 2: {
        int16_t* array16 = static_cast<int16_t*>(arrayPtr);
        for (uint32_t i = 0; i<arraySize; i++) {
            value.int16[1] = array16[i];
            buffer.emplace_back(value.int8[2]);
            buffer.emplace_back(value.int8[3]);
        }
        filled += arraySize*typeSize;
        break;
    }
    case 4: {
        int32_t* array32 = static_cast<int32_t*>(arrayPtr);
        for (uint32_t i = 0; i<arraySize; i++) {
            value.int32 = array32[i];
            buffer.emplace_back(value.int8[0]);
            buffer.emplace_back(value.int8[1]);
            buffer.emplace_back(value.int8[2]);
            buffer.emplace_back(value.int8[3]);
        }
        filled += arraySize*typeSize;
        break;
    }
    }

    return true;
}

#endif //CROSSAUDIOSTREAMMANAGER_BUFFER_BASE_INL
