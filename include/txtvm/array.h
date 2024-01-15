#ifndef TXTVM_ARRAY_H_
#define TXTVM_ARRAY_H_

#include <type_traits>
#include <vector>
#include <initializer_list>

#include "./base.h"

namespace txtvm {

    /*! \brief node content in array */
    class ArrayNode : public Node {
    public:
        /*! \brief the data content */
        std::vector<std::shared_ptr<Node> > data;
        const char* type_key() const override {
            return "ArrayNode";
        }
        void VisitAttrs(AttrVisitor* visitor) override {
            LOG(FATAL) << "need to specially handle list";
        }
    }; // class end of ArrayNode

    /*!
     * \brief Array container of NodeRef in DSL graph.
     *  Array implements copy on write semantics, which means array is mutable
     *  but copy will happen when array is referenced in more than two places.
     * \tparam T The content NodeRef type.
     */
    template<typename T,
        typename = typename std::enable_if<std::is_base_of<NodeRef, T>::value>::type >
    class Array : public NodeRef {
    public:
        /*!
        * \brief default constructor
        */
        Array() {}

        /*!
        * \brief move constructor
        * \param other source
        */
        Array(Array<T> && other) {
            node_ = std::move(other.node_);
        }

        /*!
        * \brief copy constructor
        * \param other source
        */
        Array(const Array<T> &other) {
            node_ = other.node_;
        }

        /*!
        * \brief constructor from iterator
        * \param begin begin of iterator
        * \param end end of iterator
        * \tparam IterType The type of iterator
        */
        template<typename IterType>
        Array(IterType begin, IterType end) {
            assign(begin, end);
        }

        /*!
        * \brief constructor from initializer list
        * \param init The initalizer list
        */
        Array(std::initializer_list<T> init) {
            assign(init.begin(), init.end());
        }

        /*!
        * \brief constructor from vector
        * \param init The vector
        */
        Array(const std::vector<T>& init) {
            assign(init.begin(), init.end());
        }

        /*!
        * \brief move assign operator
        * \param other The source of assignment
        * \return reference to self.
        */
        Array<T>& operator=(Array<T> && other) {
            node_ = std::move(other.node_);
            return *this;
        }

        /*!
        * \brief copy assign operator
        * \param other The source of assignment
        * \return reference to self.
        */
        Array<T>& operator=(const Array<T> & other) {
            node_ = std::move(other.node_);
            return *this;
        }

        /*!
        * \brief reset the array to content from iterator.
        * \param begin begin of iterator
        * \param end end of iterator
        * \tparam IterType The type of iterator
        */
        template<typename IterType>
        void assign(IterType begin, IterType end) {
            auto n = std::make_shared<ArrayNode>();
            n->data.reserve(end - begin);
            for (IterType i = begin; i < end; ++i) {
                n->data.push_back(i->node_);
            }
            node_ = std::move(n);
        }

        /*!
        * \brief Read i-th element from array.
        * \param i The index
        * \return the i-th element.
        */
        inline T operator[](size_t i) const {
            T inst;
            inst.node_ = static_cast<const ArrayNode*>(node_.get())->data[i];
            return inst;
        }
        /*! \return The size of the array */
        inline size_t size() const {
            if (node_.get() == nullptr) return 0;
            return static_cast<const ArrayNode*>(node_.get())->data.size();
        }

    }; // class end of Array


} // namespace txtvm


#endif // TXTVM_ARRAY_H_
