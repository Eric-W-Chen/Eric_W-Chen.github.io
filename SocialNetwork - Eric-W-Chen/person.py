class Person:
	def __init__(self, username, password, status, friends):
		self.username = username
		self.password = password
		self.status = status
		self.friends = friends

	def getUsername(self):
		return self.username

	def getPassword(self):
		return self.password

	def getStatus(self):
		return self.status

	def getFriends(self):
		return self.friends

	def setUsername(self, newUsername):
		self.username = newUsername

	def setPassword(self, newPassword):
		self.password = newPassword

	def setStatus(self, newStatus):
		self.status = newStatus

	def setFriends(self, newFriends):
		self.friends = newFriends

	def __str__(self):
		output = self.username + ', ' +  self.password + ', ' + 'messages, '
		for m in self.status:
			output += m + ', ' 
		output += "friends, "
		for n in range(len(self.friends) - 1):
			output += self.friends[n] + ', '
		output += self.friends[len(self.friends) - 1]
		return output
