import person

def main():
    newlist = []
    infile = open("network.txt", "r")
    filecontents = infile.readlines()
    usingprogram = True
    for aline in filecontents:
        info = aline.split(", ")
        name = info[0]
        password = info[1]
        friends = []
        status = []
        i = 3
        while info[i] != "friends":
            status.append(info[i])
            i += 1
        #print(status)

        i = i + 1
        while i < len(info):
            if(info[i] == ','):
                continue
            friends.append(info[i])
            i += 1

        person1 = person.Person(name, password, status, friends)
        newlist.append(person1)
    #for aperson in newlist:
       # print(aperson)
    while usingprogram == True:
        usingprogram = login(newlist, usingprogram)

def buildarray(filecontents):
    newlist = []
    for aline in filecontents:
        info = aline.split(", ")
        name = info[0]
        password = info[1]
        #print("hello")
        friends = []
        status = []
        i = 3
        while info[i] != "friends":
            status.append(info[i])
            i += 1
        #print(status)

        i = i + 1
        while i < len(info):
            friends.append(info[i])
            i += 1
       # print(friends)

        person1 = person.Person(name, password, status, friends)
        newlist.append(person1)
    return newlist


def login(newlist, usingprogram):
    inputUser = input("Enter your username: ")
    for aperson in newlist:
        print(aperson.getUsername())
        if inputUser in aperson.getUsername():
            inputPass = input("Enter your password: ")
            name = aperson.getUsername()
            password = aperson.getPassword()
            if inputPass == password:
                exit = False
                while exit == False:
                    print("1. Print all friends")
                    print("2. Print all messages/status updates")
                    print("3. Post a message/status update")
                    print("4. Print all friends'messages/status updates")
                    print("5. Add a friend")
                    print("6. Logout (Change user)")
                    print("7. Exit")

                    userchoice = 0
                    userchoice = int(input("Which action would you like to perform?"))

                    if userchoice == 1:
                        print(aperson.getFriends())

                    elif userchoice == 2:
                        print(aperson.getStatus())

                    elif userchoice == 3:
                        statusupdate = input("What would you like to post?")
                        statusarray = aperson.getStatus()
                        statusarray.append(statusupdate)
                        aperson.setStatus(statusarray)

                    elif userchoice == 4:
                        friends = aperson.getFriends()
                        for friend in friends:
                            for personfriend in newlist:
                                if personfriend.getUsername() == friend:
                                    print(personfriend.getStatus())


                    elif userchoice == 5:
                        newfriend = input("Who would you like to add?")
                        friendarray = aperson.getFriends()
                        friendarray.append(newfriend)
                        aperson.setFriends(friendarray)

                    elif userchoice == 6: 
                        return True

                    elif userchoice == 7:
                        outfile = open("network.txt", "w")
                        for aperson in newlist:
                            outfile.write(aperson.__str__())
                        return False




    #while firstupdate != "friends":
main() 