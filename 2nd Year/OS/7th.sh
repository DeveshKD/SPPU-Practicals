# Address book file
ADDRESS_BOOK="address_book.txt"

# Function to create an address book
create_address_book() {
    touch "$ADDRESS_BOOK"
    echo "Address book created."
}

# Function to view the address book
view_address_book() {
    if [ ! -f "$ADDRESS_BOOK" ]; then
        echo "Address book does not exist. Please create one first."
        return
    fi
    echo "Address Book:"
    cat "$ADDRESS_BOOK"
}

# Function to insert a record
insert_record() {
    read -p "Enter Name: " name
    read -p "Enter Phone: " phone
    read -p "Enter Email: " email
    echo "$name, $phone, $email" >> "$ADDRESS_BOOK"
    echo "Record added."
}

# Function to delete a record
delete_record() {
    read -p "Enter the name of the record to delete: " name
    sed -i "/^$name,/d" "$ADDRESS_BOOK"
    echo "Record deleted."
}

# Function to modify a record
modify_record() {
    read -p "Enter the name of the record to modify: " name
    read -p "Enter new Phone: " new_phone
    read -p "Enter new Email: " new_email
    sed -i "s/^$name,.*/$name, $new_phone, $new_email/" "$ADDRESS_BOOK"
    echo "Record modified."
}

# Main menu loop
while true; do
    echo ""
    echo "1) Create Address Book"
    echo "2) View Address Book"
    echo "3) Insert Record"
    echo "4) Delete Record"
    echo "5) Modify Record"
    echo "6) Exit"
    read -p "Choose an option (1-6): " option

    case $option in
        1) create_address_book ;;
        2) view_address_book ;;
        3) insert_record ;;
        4) delete_record ;;
        5) modify_record ;;
        6) echo "Exiting."; exit 0 ;;
        *) echo "Invalid option. Please try again." ;;
    esac
done
