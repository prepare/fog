// [Fog-Core Library - Public API]
//
// [License]
// MIT, See COPYING file in package

// [Guard]
#ifndef _FOG_CORE_HASH_H
#define _FOG_CORE_HASH_H

// [Dependencies]
#include <Fog/Build/Build.h>
#include <Fog/Core/Assert.h>
#include <Fog/Core/Atomic.h>
#include <Fog/Core/Char.h>
#include <Fog/Core/Constants.h>
#include <Fog/Core/HashUtil.h>
#include <Fog/Core/List.h>
#include <Fog/Core/Memory.h>
#include <Fog/Core/SequenceInfo.h>
#include <Fog/Core/Static.h>
#include <Fog/Core/Std.h>
#include <Fog/Core/TypeInfo.h>

namespace Fog {

//! @addtogroup Fog_Core
//! @{

// ============================================================================
// [Fog::UnorderedAbstract]
// ============================================================================

//! @brief Abstract hash container.
struct FOG_API UnorderedAbstract
{
  // --------------------------------------------------------------------------
  // [Node]
  // --------------------------------------------------------------------------

  struct FOG_HIDDEN Node
  {
    Node* next;
    uint32_t hashCode;
  };

  // --------------------------------------------------------------------------
  // [Data]
  // --------------------------------------------------------------------------

  struct FOG_HIDDEN Data
  {
    //! @brief Reference count.
    mutable Atomic<sysuint_t> refCount;

    //! @brief Count of buckets.
    sysuint_t capacity;
    //! @brief Count of nodes.
    sysuint_t length;

    //! @brief Count of buckets we will expand to if length exceeds _expandLength.
    sysuint_t expandCapacity;
    //! @brief Count of nodes to grow.
    sysuint_t expandLength;

    //! @brief Count of buckets we will shrink to if length gets _shinkLength.
    sysuint_t shrinkCapacity;
    //! @brief Count of nodes to shrink.
    sysuint_t shrinkLength;

    //! @brief Buckets.
    void* buckets[1];

    FOG_INLINE Data* ref() const
    {
      refCount.inc();
      return const_cast<Data*>(this);
    }
  };

  static Static<Data> sharedNull;

  // --------------------------------------------------------------------------
  // [Implicit Sharing]
  // --------------------------------------------------------------------------

  FOG_INLINE bool isDetached() const { return _d->refCount.get() == 1; }
  FOG_INLINE sysuint_t refCount() const { return _d->refCount.get(); }

  // --------------------------------------------------------------------------
  // [Container]
  // --------------------------------------------------------------------------

  FOG_INLINE sysuint_t getCapacity() const { return _d->capacity; }
  FOG_INLINE sysuint_t getLength() const { return _d->length; }
  FOG_INLINE bool isEmpty() const { return _d->length == 0; }

  // --------------------------------------------------------------------------
  // [Abstract Iterator]
  // --------------------------------------------------------------------------

  struct FOG_API _Iterator
  {
    FOG_INLINE _Iterator(UnorderedAbstract& hash) :
      _hash(&hash),
      _node(NULL),
      _index(INVALID_INDEX)
    {}

    FOG_INLINE _Iterator(UnorderedAbstract* hash) :
      _hash(hash),
      _node(NULL),
      _index(INVALID_INDEX)
    {}

    FOG_INLINE bool isValid() const
    { return _node != NULL; }

    void _toBegin();
    void _toNext();
    Node* _removeCurrent();

  protected:
    UnorderedAbstract* _hash;
    Node* _node;
    sysuint_t _index;
  };

  // --------------------------------------------------------------------------
  // [Helpers]
  // --------------------------------------------------------------------------

  bool _rehash(sysuint_t bc);

  static Data* _allocData(sysuint_t capacity);
  static void _freeData(Data* d);

  static sysuint_t _calcExpandCapacity(sysuint_t capacity);
  static sysuint_t _calcShrinkCapacity(sysuint_t capacity);

  // --------------------------------------------------------------------------
  // [Members]
  // --------------------------------------------------------------------------

  FOG_DECLARE_D(Data)
};

// ============================================================================
// [Fog::UnorderedHash]
// ============================================================================

template<typename KeyType, typename ValueType>
struct UnorderedHash : public UnorderedAbstract
{
  // --------------------------------------------------------------------------
  // [Node]
  // --------------------------------------------------------------------------

  struct Node
  {
    FOG_INLINE Node(uint32_t hashCode, const KeyType& key) :
      next(NULL), hashCode(hashCode), key(key)
    {
    }

    FOG_INLINE Node(uint32_t hashCode, const KeyType& key, const ValueType& value) :
      next(NULL), hashCode(hashCode), key(key), value(value)
    {
    }

    FOG_INLINE ~Node()
    {
    }

    Node* next;
    uint32_t hashCode;
    KeyType key;
    ValueType value;
  };

  // --------------------------------------------------------------------------
  // [Data]
  // --------------------------------------------------------------------------

  struct Data : public UnorderedAbstract::Data
  {
    void deref()
    {
      if (refCount.deref())
      {
        clear();
        UnorderedAbstract::_freeData(this);
      }
    }

    void clear()
    {
      sysuint_t i, len = capacity;

      for (i = 0; i < len; i++)
      {
        Node* node = (Node*)(buckets[i]);
        if (node)
        {
          do {
            Node* next = node->next;
            delete node;
            node = next;
          } while (node);
          buckets[i] = NULL;
        }
      }
    }
  };

  // --------------------------------------------------------------------------
  // [Construction / Destruction]
  // --------------------------------------------------------------------------

  UnorderedHash() { _d = sharedNull->ref(); }
  UnorderedHash(const UnorderedHash<KeyType, ValueType>& other) { _d = other._d->ref(); }
  ~UnorderedHash() { ((Data*)_d)->deref(); }

  // --------------------------------------------------------------------------
  // [Implicit Sharing]
  // --------------------------------------------------------------------------

  FOG_INLINE err_t detach() { return (!isDetached()) ? _detach() : ERR_OK; }
  err_t _detach(Node* exclude = NULL);

  // --------------------------------------------------------------------------
  // [Manipulation]
  // --------------------------------------------------------------------------

  void clear();

  bool contains(const KeyType& key) const;

  err_t put(const KeyType& key, const ValueType& value, bool replace = true);
  err_t remove(const KeyType& key);

  ValueType* get(const KeyType& key) const;
  ValueType* mod(const KeyType& key);

  Node* _getNode(const KeyType& key) const;

  ValueType value(const KeyType& key) const;
  ValueType value(const KeyType& key, const ValueType& defaultValue) const;

  List<KeyType> keys() const;
  List<KeyType> keys(const ValueType& value) const;

  UnorderedHash<KeyType, ValueType>& operator=(const UnorderedHash<KeyType, ValueType>& other)
  { ((Data*)atomicPtrXchg(&_d, other._d->ref()))->deref(); return *this; }

  FOG_INLINE ValueType* operator[](const KeyType& key)
  { return mod(key); }

  // --------------------------------------------------------------------------
  // [Const Iterator]
  // --------------------------------------------------------------------------

  struct ConstIterator : public UnorderedAbstract::_Iterator
  {
    FOG_INLINE ConstIterator(const UnorderedHash<KeyType, ValueType>& hash) :
      _Iterator((UnorderedAbstract *)&hash)
    {}

    FOG_INLINE ConstIterator(const UnorderedHash<KeyType, ValueType>* hash) :
      _Iterator((UnorderedAbstract *)hash)
    {}

    FOG_INLINE ConstIterator& toStart()
    { _toBegin(); return *this; }

    FOG_INLINE ConstIterator& toNext()
    { _toNext(); return *this; }

    FOG_INLINE const KeyType& key() const
    { return ((Node*)_node)->key; }

    FOG_INLINE const ValueType& value() const
    { return ((Node*)_node)->value; }
  };

  // --------------------------------------------------------------------------
  // [Mutable Iterator]
  // --------------------------------------------------------------------------

  struct MutableIterator : public UnorderedAbstract::_Iterator
  {
    FOG_INLINE MutableIterator(UnorderedHash<KeyType, ValueType>& hash) :
      _Iterator((UnorderedAbstract *)&hash)
    { ((UnorderedHash*)_hash)->detach(); }

    FOG_INLINE MutableIterator(UnorderedHash<KeyType, ValueType>* hash) :
      _Iterator((UnorderedAbstract *)hash)
    { ((UnorderedHash*)_hash)->detach(); }

    FOG_INLINE MutableIterator& toStart()
    { _toBegin(); return *this; }

    FOG_INLINE MutableIterator& toNext()
    { _toNext(); return *this; }

    FOG_INLINE const KeyType& key() const
    { return ((Node*)_node)->key; }

    FOG_INLINE const ValueType& value() const
    { return ((Node*)_node)->value; }

    FOG_INLINE ValueType& value()
    { return ((Node*)_node)->value; }

    FOG_INLINE void remove()
    {
      delete (Node*)_removeCurrent();
    }
  };
};

template<typename KeyType, typename ValueType>
err_t UnorderedHash<KeyType, ValueType>::_detach(Node* exclude)
{
  Data* newd = (Data*)_allocData(_d->capacity);
  if (!newd) return ERR_RT_OUT_OF_MEMORY;

  sysuint_t i, len = _d->capacity;
  sysuint_t bc = newd->capacity;

  for (i = 0; i < len; i++)
  {
    Node* node = (Node*)(_d->buckets[i]);
    while (node)
    {
      if (FOG_LIKELY(node != exclude))
      {
        uint32_t hashMod = node->hashCode % bc;
        Node* n = new(std::nothrow) Node(node->hashCode, node->key, node->value);
        if (FOG_UNLIKELY(!n)) goto alloc_fail;

        n->next = (Node*)newd->buckets[hashMod];
        newd->buckets[hashMod] = n;
      }
      node = node->next;
    }
  }

  newd->length = _d->length;
  if (exclude) newd->length--;

  ((Data*)atomicPtrXchg(&_d, (UnorderedAbstract::Data*)newd))->deref();
  return ERR_OK;

alloc_fail:
  newd->clear();
  _freeData(newd);
  return ERR_RT_OUT_OF_MEMORY;
}

template<typename KeyType, typename ValueType>
void UnorderedHash<KeyType, ValueType>::clear()
{
  ((Data*)atomicPtrXchg(&_d, sharedNull->ref()))->deref();
}

template<typename KeyType, typename ValueType>
bool UnorderedHash<KeyType, ValueType>::contains(const KeyType& key) const
{
  if (!_d->length) return false;

  uint32_t hashCode = HashUtil::getHashCode(key);
  uint32_t hashMod = hashCode % getCapacity();

  Node* node = (Node*)(_d->buckets[hashMod]);

  while (node)
  {
    if (node->hashCode == hashCode && node->key == key) return true;
    node = node->next;
  }
  return false;
}

template<typename KeyType, typename ValueType>
err_t UnorderedHash<KeyType, ValueType>::put(const KeyType& key, const ValueType& value, bool replace)
{
  if (!_d->length && !_rehash(32)) return ERR_RT_OUT_OF_MEMORY;

  detach();

  uint32_t hashCode = HashUtil::getHashCode(key);
  uint32_t hashMod = hashCode % getCapacity();

  Node* node = (Node*)(_d->buckets[hashMod]);
  Node* prev = NULL;

  while (node)
  {
    if (node->hashCode == hashCode && node->key == key) break;
    prev = node;
    node = node->next;
  }

  if (node)
  {
    if (!replace) return ERR_RT_OBJECT_ALREADY_EXISTS;
    node->value = value;
  }
  else
  {
    node = new(std::nothrow) Node(hashCode, key, value);
    if (!node) return ERR_RT_OUT_OF_MEMORY;

    if (prev)
      prev->next = node;
    else
      _d->buckets[hashMod] = node;
    if (++_d->length >= _d->expandLength) _rehash(_d->expandCapacity);
  }
  return ERR_OK;
}

template<typename KeyType, typename ValueType>
err_t UnorderedHash<KeyType, ValueType>::remove(const KeyType& key)
{
  if (!_d->length) return ERR_RT_OBJECT_NOT_FOUND;

  uint32_t hashCode = HashUtil::getHashCode(key);
  uint32_t hashMod = hashCode % getCapacity();

  Node* node = (Node*)(_d->buckets[hashMod]);
  Node* prev = NULL;

  while (node)
  {
    if (node->hashCode == hashCode && node->key == key)
    {
      if (FOG_UNLIKELY(!isDetached())) return _detach(node);

      if (prev)
        prev->next = node->next;
      else
        _d->buckets[hashMod] = node->next;

      delete node;
      if (--_d->length <= _d->shrinkLength) _rehash(_d->shrinkCapacity);
      return ERR_OK;
    }
    prev = node;
    node = node->next;
  }

  return ERR_RT_OBJECT_NOT_FOUND;
}

template<typename KeyType, typename ValueType>
ValueType* UnorderedHash<KeyType, ValueType>::get(const KeyType& key) const
{
  if (!_d->length) return NULL;

  uint32_t hashCode = HashUtil::getHashCode(key);
  uint32_t hashMod = hashCode % getCapacity();

  Node* node = (Node*)(_d->buckets[hashMod]);

  while (node)
  {
    if (node->hashCode == hashCode && node->key == key)
    {
      return &node->value;
    }
    node = node->next;
  }
  return NULL;
}

template<typename KeyType, typename ValueType>
ValueType* UnorderedHash<KeyType, ValueType>::mod(const KeyType& key)
{
  if (!_d->length && !_rehash(32)) return NULL;

  detach();

  uint32_t hashCode = HashUtil::getHashCode(key);
  uint32_t hashMod = hashCode % getCapacity();

  Node* node = (Node*)(_d->buckets[hashMod]);
  Node* prev = NULL;

  while (node)
  {
    if (node->hashCode == hashCode && node->key == key) break;
    prev = node;
    node = node->next;
  }

  if (!node)
  {
    node = new(std::nothrow) Node(hashCode, key);
    if (!node) return NULL;

    if (prev)
      prev->next = node;
    else
      _d->buckets[hashMod] = node;
    if (++_d->length >= _d->expandLength) _rehash(_d->expandCapacity);
  }

  return &node->value;
}

template<typename KeyType, typename ValueType>
typename UnorderedHash<KeyType, ValueType>::Node* UnorderedHash<KeyType, ValueType>::_getNode(const KeyType& key) const
{
  if (!_d->length) return NULL;

  uint32_t hashCode = HashUtil::getHashCode(key);
  uint32_t hashMod = hashCode % getCapacity();

  Node* node = (Node*)(_d->buckets[hashMod]);

  while (node)
  {
    if (node->hashCode == hashCode && node->key == key) return node;
    node = node->next;
  }
  return NULL;
}

template<typename KeyType, typename ValueType>
ValueType UnorderedHash<KeyType, ValueType>::value(const KeyType& key) const
{
  const ValueType* vptr = get(key);
  return vptr ? ValueType(*vptr) : ValueType();
}

template<typename KeyType, typename ValueType>
ValueType UnorderedHash<KeyType, ValueType>::value(const KeyType& key, const ValueType& defaultValue) const
{
  const ValueType* vptr = get(key);
  return ValueType(vptr ? *vptr : defaultValue);
}

template<typename KeyType, typename ValueType>
List<KeyType> UnorderedHash<KeyType, ValueType>::keys() const
{
  List<KeyType> result;
  sysuint_t i, len = _d->capacity;

  result.reserve(len);

  for (i = 0; i < len; i++)
  {
    Node* node = (Node*)(_d->buckets[i]);
    while (node)
    {
      result.append(node->key);
      node = node->next;
    }
  }

  return result;
}

template<typename KeyType, typename ValueType>
List<KeyType> UnorderedHash<KeyType, ValueType>::keys(const ValueType& value) const
{
  List<KeyType> result;
  sysuint_t i, len = _d->capacity;
  for (i = 0; i < len; i++)
  {
    Node* node = (Node*)(_d->buckets[i]);
    while (node)
    {
      if (node->value == value)
        result.append(node->key);
      node = node->next;
    }
  }

  return result;
}

// ============================================================================
// [Fog::UnorderedSet<KeyType>]
// ============================================================================

template<typename KeyType>
struct UnorderedSet : public UnorderedAbstract
{
  // --------------------------------------------------------------------------
  // [Node]
  // --------------------------------------------------------------------------

  struct Node
  {
    FOG_INLINE Node(uint32_t hashCode, const KeyType& key) :
      next(NULL), hashCode(hashCode), key(key)
    {
    }

    FOG_INLINE ~Node()
    {
    }

    Node* next;
    uint32_t hashCode;
    KeyType key;
  };

  // --------------------------------------------------------------------------
  // [Data]
  // --------------------------------------------------------------------------

  struct Data : public UnorderedAbstract::Data
  {
    void deref()
    {
      if (refCount.deref())
      {
        clear();
        UnorderedAbstract::_freeData(this);
      }
    }

    void clear()
    {
      sysuint_t i, len = capacity;

      for (i = 0; i < len; i++)
      {
        Node* node = (Node*)(buckets[i]);
        if (node)
        {
          do {
            Node* next = node->next;
            delete node;
            node = next;
          } while (node);
          buckets[i] = NULL;
        }
      }
    }
  };

  // --------------------------------------------------------------------------
  // [Construction / Destruction]
  // --------------------------------------------------------------------------

  UnorderedSet() { _d = sharedNull->ref(); }
  UnorderedSet(const UnorderedSet<KeyType>& other) { _d = other._d->ref(); }
  ~UnorderedSet() { ((Data*)_d)->deref(); }

  // --------------------------------------------------------------------------
  // [Implicit Sharing]
  // --------------------------------------------------------------------------

  FOG_INLINE err_t detach() { return (!isDetached()) ? _detach() : ERR_OK; }
  err_t _detach(Node* exclude = NULL);

  // --------------------------------------------------------------------------
  // [Manipulation]
  // --------------------------------------------------------------------------

  void clear();

  bool contains(const KeyType& key) const;

  err_t put(const KeyType& key);
  err_t remove(const KeyType& key);

  Node* _getNode(const KeyType& key) const;

  List<KeyType> keys() const;

  UnorderedSet<KeyType>& operator=(const UnorderedSet<KeyType>& other)
  { ((Data*)atomicPtrXchg(&_d, other._d->ref()))->deref(); return *this; }

  // --------------------------------------------------------------------------
  // [Const Iterator]
  // --------------------------------------------------------------------------

  struct ConstIterator : public UnorderedAbstract::_Iterator
  {
    FOG_INLINE ConstIterator(const UnorderedSet<KeyType>& hash) :
      _Iterator((UnorderedAbstract *)&hash)
    {}

    FOG_INLINE ConstIterator(const UnorderedSet<KeyType>* hash) :
      _Iterator((UnorderedAbstract *)hash)
    {}

    FOG_INLINE ConstIterator& toStart()
    { _toBegin(); return *this; }

    FOG_INLINE ConstIterator& toNext()
    { _toNext(); return *this; }

    FOG_INLINE const KeyType& key() const
    { return ((Node*)_node)->key; }
  };

  // --------------------------------------------------------------------------
  // [Mutable Iterator]
  // --------------------------------------------------------------------------

  struct MutableIterator : public UnorderedAbstract::_Iterator
  {
    FOG_INLINE MutableIterator(UnorderedSet<KeyType>& hash) :
      _Iterator((UnorderedAbstract *)&hash)
    { ((UnorderedSet*)_hash)->detach(); }

    FOG_INLINE MutableIterator(UnorderedSet<KeyType>* hash) :
      _Iterator((UnorderedAbstract *)hash)
    { ((UnorderedSet*)_hash)->detach(); }

    FOG_INLINE MutableIterator& toStart()
    { _toBegin(); return *this; }

    FOG_INLINE MutableIterator& toNext()
    { _toNext(); return *this; }

    FOG_INLINE const KeyType& key() const
    { return ((Node*)_node)->key; }

    FOG_INLINE void remove()
    {
      delete (Node*)_removeCurrent();
    }
  };
};

template<typename KeyType>
err_t UnorderedSet<KeyType>::_detach(Node* exclude)
{
  Data* newd = (Data*)_allocData(_d->capacity);
  if (!newd) return ERR_RT_OUT_OF_MEMORY;

  sysuint_t i, len = _d->capacity;
  sysuint_t bc = newd->capacity;

  for (i = 0; i < len; i++)
  {
    Node* node = (Node*)(_d->buckets[i]);
    while (node)
    {
      if (FOG_LIKELY(node != exclude))
      {
        uint32_t hashMod = node->hashCode % bc;
        Node* n = new(std::nothrow) Node(node->hashCode, node->key, node->value);
        if (FOG_UNLIKELY(!n)) goto alloc_fail;

        n->next = (Node*)newd->buckets[hashMod];
        newd->buckets[hashMod] = n;
      }
      node = node->next;
    }
  }

  newd->length = _d->length;
  if (exclude) newd->length--;

  ((Data*)atomicPtrXchg(&_d, (UnorderedAbstract::Data*)newd))->deref();
  return ERR_OK;

alloc_fail:
  newd->clear();
  _freeData(newd);
  return ERR_RT_OUT_OF_MEMORY;
}

template<typename KeyType>
void UnorderedSet<KeyType>::clear()
{
  ((Data*)atomicPtrXchg(&_d, sharedNull->ref()))->deref();
}

template<typename KeyType>
bool UnorderedSet<KeyType>::contains(const KeyType& key) const
{
  if (!_d->length) return false;

  uint32_t hashCode = HashUtil::getHashCode(key);
  uint32_t hashMod = hashCode % getCapacity();

  Node* node = (Node*)(_d->buckets[hashMod]);

  while (node)
  {
    if (node->hashCode == hashCode && node->key == key) return true;
    node = node->next;
  }
  return false;
}

template<typename KeyType>
err_t UnorderedSet<KeyType>::put(const KeyType& key)
{
  if (!_d->length && !_rehash(32)) return ERR_RT_OUT_OF_MEMORY;

  detach();

  uint32_t hashCode = HashUtil::getHashCode(key);
  uint32_t hashMod = hashCode % getCapacity();

  Node* node = (Node*)(_d->buckets[hashMod]);
  Node* prev = NULL;

  while (node)
  {
    if (node->hashCode == hashCode && node->key == key) break;
    prev = node;
    node = node->next;
  }

  if (node)
  {
    if (!replace) return ERR_RT_OBJECT_ALREADY_EXISTS;
    node->value = value;
  }
  else
  {
    node = new(std::nothrow) Node(hashCode, key, value);
    if (!node) return ERR_RT_OUT_OF_MEMORY;

    if (prev)
      prev->next = node;
    else
      _d->buckets[hashMod] = node;
    if (++_d->length >= _d->expandLength) _rehash(_d->expandCapacity);
  }
  return ERR_OK;
}

template<typename KeyType>
err_t UnorderedSet<KeyType>::remove(const KeyType& key)
{
  if (!_d->length) return ERR_RT_OBJECT_NOT_FOUND;

  uint32_t hashCode = HashUtil::getHashCode(key);
  uint32_t hashMod = hashCode % getCapacity();

  Node* node = (Node*)(_d->buckets[hashMod]);
  Node* prev = NULL;

  while (node)
  {
    if (node->hashCode == hashCode && node->key == key)
    {
      if (FOG_UNLIKELY(!isDetached())) return _detach(node);

      if (prev)
        prev->next = node->next;
      else
        _d->buckets[hashMod] = node->next;

      delete node;
      if (--_d->length <= _d->shrinkLength) _rehash(_d->shrinkCapacity);
      return true;
    }
    prev = node;
    node = node->next;
  }

  return ERR_RT_OBJECT_NOT_FOUND;
}

template<typename KeyType>
typename UnorderedSet<KeyType>::Node* UnorderedSet<KeyType>::_getNode(const KeyType& key) const
{
  if (!_d->length) return NULL;

  uint32_t hashCode = HashUtil::getHashCode(key);
  uint32_t hashMod = hashCode % getCapacity();

  Node* node = (Node*)(_d->buckets[hashMod]);

  while (node)
  {
    if (node->hashCode == hashCode && node->key == key) return node;
    node = node->next;
  }
  return NULL;
}

template<typename KeyType>
List<KeyType> UnorderedSet<KeyType>::keys() const
{
  List<KeyType> result;
  sysuint_t i, len = _d->capacity;

  result.reserve(len);

  for (i = 0; i < len; i++)
  {
    Node* node = (Node*)(_d->buckets[i]);
    while (node)
    {
      result.append(node->key);
      node = node->next;
    }
  }

  return result;
}

//! @}

} // Fog namespace

// ============================================================================
// [Fog::TypeInfo<>]
// ============================================================================

FOG_DECLARE_TYPEINFO_TEMPLATE2(Fog::UnorderedHash,
  typename, KeyType,
  typename, ValueType,
  Fog::TYPEINFO_MOVABLE)

FOG_DECLARE_TYPEINFO_TEMPLATE1(Fog::UnorderedSet,
  typename, KeyType,
  Fog::TYPEINFO_MOVABLE)

// [Guard]
#endif // _FOG_CORE_HASH_H
